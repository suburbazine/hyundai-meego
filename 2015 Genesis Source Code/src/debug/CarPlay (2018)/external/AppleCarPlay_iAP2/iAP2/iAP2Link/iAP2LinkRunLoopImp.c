/*
 *	File: iAP2LinkRunLoopImp.c
 *	Package: iAP2Link
 *	Abstract: n/a 
 *
 *	Disclaimer: IMPORTANT: This Apple software is supplied to you, by Apple
 * 	Inc. ("Apple"), in your capacity as a current, and in good standing,
 *	Licensee in the MFi Licensing Program. Use of this Apple software is
 *	governed by and subject to the terms and conditions of your MFi License,
 *	including, but not limited to, the restrictions specified in the provision
 *	entitled “Public Software”, and is further subject to your agreement to
 *	the following additional terms, and your agreement that the use,
 *	installation, modification or redistribution of this Apple software
 * 	constitutes acceptance of these additional terms. If you do not agree with
 * 	these additional terms, please do not use, install, modify or redistribute
 *	this Apple software.
 *
 *	In consideration of your agreement to abide by the following terms, and
 *	subject to these terms, Apple grants you a personal, non-exclusive
 *	license, under Apple's copyrights in this original Apple software (the
 *	"Apple Software"), to use, reproduce, and modify the Apple Software in
 *	source form, and to use, reproduce, modify, and redistribute the Apple
 *	Software, with or without modifications, in binary form. While you may not
 *	redistribute the Apple Software in source form, should you redistribute
 *	the Apple Software in binary form, in its entirety and without
 *	modifications, you must retain this notice and the following text and
 *	disclaimers in all such redistributions of the Apple Software. Neither the
 *	name, trademarks, service marks, or logos of Apple Inc. may be used to
 *	endorse or promote products derived from the Apple Software without
 *	specific prior written permission from Apple. Except as expressly stated
 *	in this notice, no other rights or licenses, express or implied, are
 *	granted by Apple herein, including but not limited to any patent rights
 *	that may be infringed by your derivative works or by other works in which
 *	the Apple Software may be incorporated.
 *	
 *	The Apple Software is provided by Apple on an "AS IS" basis. APPLE MAKES
 *	NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE
 *	IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
 *	PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION
 *	ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 *
 *	IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
 *	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *	INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
 *	MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND
 *	WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT
 *	LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE POSSIBILITY
 *	OF SUCH DAMAGE.
 *
 *	Copyright (C) 2012 Apple Inc. All Rights Reserved.
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include <iAP2LinkConfig.h>
#include <iAP2Time.h>
#include <iAP2Log.h>

#include "iAP2LinkRunLoop.h"
#include "iAP2Packet.h"
#include "iAP2Link.h"
#include "iAP2LinkRunLoopImp.h"
#include "iAP2TimeImplementation.h"
#include <pthread.h>

void initContext(IAPServiceContext_t* pContext)
{
	LOGD("initContext() pIApContext[%p]", pContext);

	pContext->bThreadTerminate = FALSE;	//#LGE_CHANGE yongil.park Problem : Set bThreadTerminate after free pIApContext sometimes
	pthread_mutex_init(&pContext->iAP2LinkRLTMutex, NULL);
	pthread_cond_init(&pContext->iAP2LinkRLTCond, NULL);

	pthread_mutex_init(&pContext->eventMaskMutex, NULL);

	pthread_mutex_init(&pContext->iAP2LinkListMutex, NULL); //#LGE_CHAGE Yongil.park Problem : packet value is null in the send-packet list

	
	//pthread_mutex_init(&pContext->timerMutex, NULL);

	pContext->pRcvPacketQHead = NULL;
	pContext->pRcvPacketQTail = NULL;

	pContext->pExpiredTimerQHead = NULL;
	pContext->pExpiredTimerQTail = NULL;	

	queueInit(&pContext->pRcvPacketQHead, &pContext->pRcvPacketQTail);
	queueInit(&pContext->pExpiredTimerQHead, &pContext->pExpiredTimerQTail);

	listInitNode(&pContext->pTimerListHead, &pContext->pTimerListTail);
	
}

void finalContext(IAPServiceContext_t* pContext)
{
    LOGD("finalContext()");

    if (pContext == NULL)
    {
        LOGE("ERROR : pContext is NULL");
        return;
    }

    //pthread_mutex_destroy(&pContext->timerMutex);
    pthread_mutex_lock(&pContext->iAP2LinkRLTMutex);
    queueFinal(pContext->pRcvPacketQHead, pContext->pRcvPacketQTail, TRUE);
    pthread_mutex_unlock(&pContext->iAP2LinkRLTMutex);
    LOGD("### free RcvPacket Complete ###");
	
    pthread_mutex_lock(&pContext->iAP2LinkRLTMutex);    
    queueFinal(pContext->pExpiredTimerQHead, pContext->pExpiredTimerQTail, TRUE);
    pthread_mutex_unlock(&pContext->iAP2LinkRLTMutex);
    LOGD("### free ExpiredTimer Complete ###");

    pthread_mutex_lock(&pContext->iAP2LinkRLTMutex);        
    queueFinal(pContext->pTimerListHead, pContext->pTimerListTail, TRUE);
    pthread_mutex_unlock(&pContext->iAP2LinkRLTMutex);
    LOGD("### free pTimerList Complete ###");

    pContext->bThreadTerminate = FALSE; //#LGE_CHANGE yongil.park Problem : Set bThreadTerminate after free pIApContext sometimes

    pthread_mutex_destroy(&pContext->iAP2LinkRLTMutex);
    pthread_cond_destroy(&pContext->iAP2LinkRLTCond);

    pthread_mutex_destroy(&pContext->eventMaskMutex);
    pthread_mutex_destroy(&pContext->iAP2LinkListMutex); //#LGE_CHAGE Yongil.park Problem : packet value is null in the send-packet list


    if (pContext != NULL){
        LOGD("### free pContext ###");
        free(pContext);
    }
}

void processTimer(IAPServiceContext_t* pIApContext)
{
	iAP2Timer_t* pTimer;
	TimerContext_t* pTimerCtx;

	pthread_mutex_lock(&pIApContext->iAP2LinkRLTMutex);
	pTimer = queueGet(pIApContext->pExpiredTimerQHead, pIApContext->pExpiredTimerQTail);
	pthread_mutex_unlock(&pIApContext->iAP2LinkRLTMutex);

	if (pTimer != NULL)
	{
		pTimerCtx = timerListGetTimerContext(pTimer);
		if (pTimerCtx == NULL)
			LOGW("WARNING : pTimerCtx is NULL", pTimerCtx);
		else
		{
			uint32_t curTime = iAP2TimeGetCurTimeMs();
			//LOGD("==TIMER<< call pTimerCtx->callback[%p] pTimer[%p] piAP2Timer[%p] curTime[%u]", pTimerCtx->callback, pTimer, pTimerCtx->piAP2Timer, curTime);
			pTimerCtx->callback(pTimer, curTime);
		}
	}	
}

void retreiveRcvPacket(IAPServiceContext_t* pIApContext, void** ppRcvPacket, unsigned int* pnRcvPacket)
{
	// get recieved packet in the QUEUE
	pthread_mutex_lock(&pIApContext->iAP2LinkRLTMutex);
	*pnRcvPacket = queueGetCount(pIApContext->pRcvPacketQHead, pIApContext->pRcvPacketQTail);
	*ppRcvPacket = queueGet(pIApContext->pRcvPacketQHead, pIApContext->pRcvPacketQTail);
	//*pnRcvPacket = queueGetCount(pIApContext->pRcvPacketQHead, pIApContext->pRcvPacketQTail);
	pthread_mutex_unlock(&pIApContext->iAP2LinkRLTMutex);
}

static void* iAP2LinkRunLoopThread(void* data)
{
	BOOL bRetValue = FALSE;
	IAPServiceContext_t* pIApContext = NULL;
	iAP2LinkRunLoop_t* pLinkRunLoop = (iAP2LinkRunLoop_t*) data;

	if (pLinkRunLoop == NULL) {
		LOGE("ERROR : linkRunLoop is NULL");
		pthread_exit(NULL);
		return NULL;
	}

	pIApContext = (IAPServiceContext_t*)pLinkRunLoop->otherData;
	if (pIApContext == NULL)
	{
		LOGE("ERROR : pIApContext is NULL");
		pthread_exit(NULL);
	}

	bRetValue = iAP2LinkRunLoopRun(pLinkRunLoop);

	LOGD("######################## bRetValue[%d] shuttingDown[%d] #######################", bRetValue, pLinkRunLoop->shuttingDown);   	
	
	pIApContext->bThreadTerminate = TRUE;

	 //#LGE_CHANGE_START yongil.park Problem : Set bThreadTerminate after free pIApContext sometimes
	iAP2LinkDelete (pLinkRunLoop->link);
       pLinkRunLoop->link = NULL;
	iAP2LinkRunLoopCleanupImplementation (pLinkRunLoop); 
	//#LGE_CHANGE_END
 
	pthread_exit(NULL);
	return NULL;
}

/*
*****************************************************************
**
**  iAP2LinkRunLoopInitImplementation
**
**  Input:
**      linkRunLoop:    linkRunLoop structure to delete
**
**  Output:
**      None
**
**  Return:
**      None
**
**  Note:   Implementation specific initialization.
**          Implementation specific data can be stored in iAP2LinkRunLoop_t.otherData
**          TODO: Needs to be implemented by the user of iAP2LinkRunLoop
**
**
*****************************************************************
*/
void iAP2LinkRunLoopInitImplementation (iAP2LinkRunLoop_t* linkRunLoop)
{
	int retValue = 0;
	IAPServiceContext_t* pIApContext = NULL;

	if (linkRunLoop == NULL)
	{
		LOGE("ERROR : linkRunLoop is NULL");
		return;
	}

    pIApContext = (IAPServiceContext_t*)linkRunLoop->otherData;
	if (pIApContext == NULL)
	{
		LOGE("ERROR : pIApContext is NULL");
		return;
	}

	LOGD("called iAP2LinkRunLoopInitImplementation() linkRunLoop[%p] pIApContext[%p]", linkRunLoop, pIApContext);

	retValue = pthread_create(&pIApContext->hThread, NULL, iAP2LinkRunLoopThread, (void*)linkRunLoop);

	if (retValue != 0)
		LOGE("ERROR : fail to create pthread retValue[%d]", retValue);
}


/*
*****************************************************************
**
**  iAP2LinkRunLoopCleanupImplementation
**
**  Input:
**      linkRunLoop:    linkRunLoop structure to delete
**
**  Output:
**      None
**
**  Return:
**      None
**
**  Note:   Implementation specific cleanup.
**          TODO: Needs to be implemented by the user of iAP2LinkRunLoop
**
**
*****************************************************************
*/
void iAP2LinkRunLoopCleanupImplementation (iAP2LinkRunLoop_t* linkRunLoop)
{
	IAPServiceContext_t* pIApContext = NULL;

	if (linkRunLoop == NULL)
	{
		LOGE("ERROR : linkRunLoop is NULL");
		return;
	}

    	pIApContext = (IAPServiceContext_t*)linkRunLoop->otherData;
	if (pIApContext == NULL)
	{
		LOGE("ERROR : pIApContext is NULL");
		return;
	}

	if (pIApContext->bThreadTerminate == FALSE)
		LOGE("ERROR : iAP2LinkRunLoopThread is alive yet");

	finalContext(pIApContext);
		
 
}


/*
*****************************************************************
**
**  iAP2LinkRunLoopWait
**
**  Input:
**      linkRunLoop:    linkRunLoop structure to delete
**
**  Output:
**      None
**
**  Return:
**      BOOL    TRUE if successfully waited and got signal, else FALSE
**
**  Note:   Waits for a signal (via iAP2LinkRunLoopSignal) before continuing.
**          TODO: Needs to be implemented by the user of iAP2LinkRunLoop
**
**
*****************************************************************
*/
BOOL iAP2LinkRunLoopWait (iAP2LinkRunLoop_t* linkRunLoop)
{
	IAPServiceContext_t* pIApContext = NULL;

	if (linkRunLoop == NULL)
	{
		LOGE("ERROR : linkRunLoop is NULL");
		return FALSE;
	}

    pIApContext = (IAPServiceContext_t*)linkRunLoop->otherData;
	if (pIApContext == NULL)
	{
		LOGE("ERROR : pIApContext is NULL");
		return FALSE;
	}

	//LOGE("called iAP2LinkRunLoopWait(). wait ...");

    pthread_mutex_lock(&pIApContext->iAP2LinkRLTMutex);
    pthread_cond_wait(&pIApContext->iAP2LinkRLTCond, &pIApContext->iAP2LinkRLTMutex);
    pthread_mutex_unlock(&pIApContext->iAP2LinkRLTMutex);	

    return TRUE;
}


/*
*****************************************************************
**
**  iAP2LinkRunLoopSignal
**
**  Input:
**      linkRunLoop:    linkRunLoop structure to delete
**      arg:            argument to call back iAP2LinkRunLoopRunOnce with.
**
**  Output:
**      None
**
**  Return:
**      None
**
**  Note:   Signals RunLoop to continue processing (wait is via iAP2LinkRunLoopWait.
**          TODO: Needs to be implemented by the user of iAP2LinkRunLoop
**
*****************************************************************
*/
void iAP2LinkRunLoopSignal (iAP2LinkRunLoop_t* linkRunLoop, void* arg)
{
	IAPServiceContext_t* pIApContext = NULL;

	if (linkRunLoop == NULL)
	{
		LOGE("ERROR : linkRunLoop is NULL");
		return;
	}

	pIApContext = (IAPServiceContext_t*)linkRunLoop->otherData;
	if (pIApContext == NULL)
	{
		LOGE("ERROR : pIApContext is NULL");
		return;
	}

	//LOGD("called iAP2LinkRunLoopSignal()");

    pthread_mutex_lock(&pIApContext->iAP2LinkRLTMutex);

   	// put the arg in into Received Packet QUEUE
#if 1
	queuePut(pIApContext->pRcvPacketQHead, pIApContext->pRcvPacketQTail, arg);
#else
   	if (arg != NULL)
		queuePut(pIApContext->pRcvPacketQHead, pIApContext->pRcvPacketQTail, arg);
#endif
    pthread_cond_signal(&pIApContext->iAP2LinkRLTCond);
    pthread_mutex_unlock(&pIApContext->iAP2LinkRLTMutex);
}


/*
*****************************************************************
**
**  iAP2LinkRunLoopProtectedCall
**
**  Input:
**      linkRunLoop:    linkRunLoop structure to delete
**      arg:            argument for use when calling func
**      func:           function to call
**
**  Output:
**      None
**
**  Return:
**      BOOL    return value of function called with protection.
**
**  Note:   Call the indicated function with simultaneous call protection.
**          TODO: Needs to be implemented by the user of iAP2LinkRunLoop
**
*****************************************************************
*/
BOOL iAP2LinkRunLoopProtectedCall (iAP2LinkRunLoop_t* linkRunLoop,
                                   void* arg,
                                   BOOL (*func)(iAP2LinkRunLoop_t* linkRunLoop, void* arg))
{
	// It's not necessary to use Mutex becase this function is only called in iAP2LinkRunLoopThread.
    return func(linkRunLoop, arg);
    //return TRUE;
}


/*
*****************************************************************
**
**  iAP2LinkRunLoopSetEventMaskBit
**
**  Input:
**      linkRunLoop:    linkRunLoop structure to delete
**      bit:            eventMask bit to set (iAP2LinkRunLoopEventMask_t)
**
**  Output:
**      None
**
**  Return:
**      None
**
**  Note:   TODO: Needs to be implemented by the user of iAP2LinkRunLoop
**                If access to evenMask needs to be protected must make
**                sure operation on linkRunLoop->eventMask is atomic.
**
*****************************************************************
*/
void iAP2LinkRunLoopSetEventMaskBit (iAP2LinkRunLoop_t*         linkRunLoop,
                                     iAP2LinkRunLoopEventMask_t bit)
{
	IAPServiceContext_t* pIApContext = NULL;
	
	if (linkRunLoop == NULL)
	{
		LOGE("ERROR : linkRunLoop is NULL");
		return;
	}

    pIApContext = (IAPServiceContext_t*)linkRunLoop->otherData;
	if (pIApContext == NULL)
	{
		LOGE("ERROR : pIApContext is NULL");
		return;
	}

	pthread_mutex_lock(&pIApContext->eventMaskMutex);
    linkRunLoop->eventMask |= bit;
	pthread_mutex_unlock(&pIApContext->eventMaskMutex);
}

/*
*****************************************************************
**
**  iAP2LinkRunLoopGetResetEventMask
**
**  Input:
**      linkRunLoop:    linkRunLoop structure to delete
**
**  Output:
**      None
**
**  Return:
**      uint32_t    bit mask of events currently pending (iAP2LinkRunLoopEventMask_t)
**
**  Note:   Retrieve the eventMask and reset it to kiAP2LinkRunLoopEventMaskNone.
**          TODO: Needs to be implemented by the user of iAP2LinkRunLoop
**                If access to evenMask needs to be protected must make
**                sure operation on linkRunLoop->eventMask is atomic.
**
*****************************************************************
*/
uint32_t iAP2LinkRunLoopGetResetEventMask (iAP2LinkRunLoop_t* linkRunLoop)
{
	uint32_t eventMask = 0;
	IAPServiceContext_t* pIApContext = NULL;
	
	if (linkRunLoop == NULL)
	{
		LOGE("ERROR : linkRunLoop is NULL");
		return 0;
	}

    pIApContext = (IAPServiceContext_t*)linkRunLoop->otherData;
	if (pIApContext == NULL)
	{
		LOGE("ERROR : pIApContext is NULL");
		return 0;
	}

	pthread_mutex_lock(&pIApContext->eventMaskMutex);
    eventMask = linkRunLoop->eventMask;
    linkRunLoop->eventMask = kiAP2LinkRunLoopEventMaskNone;
	pthread_mutex_unlock(&pIApContext->eventMaskMutex);

    return eventMask;
}

