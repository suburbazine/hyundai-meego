/*
 *	File: iAP2TimeImplementation.h.c
 *	Package: iAP2Utility
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
#include <sys/time.h>
#include <iAP2Defines.h>
#include <iAP2Time.h>
#include <iAP2ListArray.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <iAP2LinkRunLoop.h>
#include <pthread.h>

#include "iAp2Log.h"
#include "iAP2LinkRunLoopImp.h"
#include "iAP2TimeImplementation.h"


#define MY_TIMER_SIGNAL		SIGRTMIN

// changed timer notify method from signal to thread // soohwan.oh - 2015-01-17
// signal handler includes pthread_mutex_* functions which is not signal safe
#define _SIGNAL 1
#define _THREAD 2
#define TIMER_NOTI _THREAD

/*
****************************************************************
**
** User implemented functions
**
****************************************************************
*/

static IAPServiceContext_t* _getServiceContext(iAP2Timer_t* piAP2Timer)
{
	IAPServiceContext_t* pIApContext;
	iAP2Link_t* link = iAP2TimeGetContext(piAP2Timer);

	if (link == NULL) {
		LOGE("ERROR : link is null");
		return NULL;
	}

	if (link->context == NULL) {
		LOGE("ERROR : link->context is null");
		return FALSE;
	}


	pIApContext = (IAPServiceContext_t*)((iAP2LinkRunLoop_t*)link->context)->otherData;
	return pIApContext;
}

#if (TIMER_NOTI == _SIGNAL)
static int _timerGetEmptyUserSigal(IAPServiceContext_t* pIApContext)
{
	int signalNo = MY_TIMER_SIGNAL;
	TimerContext_t* pTimerCtx = NULL;

	if (pIApContext == NULL) {
		LOGE("ERROR : pIApContext is null");
		return MY_TIMER_SIGNAL;
	}

	for (signalNo = MY_TIMER_SIGNAL; signalNo <= SIGRTMAX; signalNo++) {
		pTimerCtx = (TimerContext_t*) listGetNodeAfterNode(pIApContext->pTimerListHead, pIApContext->pTimerListTail, pIApContext->pTimerListHead);
		if (pTimerCtx == NULL) {
			LOGD("assigned signalNo[%d]", signalNo);
			return signalNo;
		}

		while (pTimerCtx) {
	        pTimerCtx = (TimerContext_t*) listGetNodeAfterNode(pIApContext->pTimerListHead, pIApContext->pTimerListTail, (dnode_t*)pTimerCtx);
			if (pTimerCtx == NULL) {
				LOGD("assigned signalNo[%d]", signalNo);
				return signalNo;
			}

			if (pTimerCtx->siSigno == signalNo) {
				LOGD("already assigned signalNo[%d]", signalNo);
				break;
			}
        } 
	}

	LOGE("ERROR : user signal is not available signalNo[%d]", signalNo);
	return SIGRTMAX;
}
#endif

TimerContext_t* timerListGetTimerContext(iAP2Timer_t* piAP2Timer)
{
	TimerContext_t* pTimerCtx = NULL;
	IAPServiceContext_t* pIApContext = _getServiceContext(piAP2Timer);

	if (pIApContext == NULL) {
		LOGE("ERROR : pIApContext is null");
		return NULL;
	}
	
	pTimerCtx = (TimerContext_t*) listGetNodeAfterNode(pIApContext->pTimerListHead, pIApContext->pTimerListTail, pIApContext->pTimerListHead);

	while (pTimerCtx) {
		//LOGD("%s: pTimerCtx_%p, piAP2Timer_%p ?= input_%p", __FUNCTION__, pTimerCtx, pTimerCtx->piAP2Timer, piAP2Timer);
        if (pTimerCtx->piAP2Timer == piAP2Timer)
			return pTimerCtx;

        pTimerCtx = (TimerContext_t*) listGetNodeAfterNode(pIApContext->pTimerListHead, pIApContext->pTimerListTail, (dnode_t*)pTimerCtx);
	}

    return NULL;
}


#if (TIMER_NOTI == _SIGNAL)
static void timerHandler(int sig, siginfo_t *si, void *context)
{
	IAPServiceContext_t* pIApContext = NULL;
	TimerContext_t* pTimerCtx = NULL;
	uint32_t curTime = 0;

	if (si == NULL) {
		LOGE("ERROR : si is NULL");
		return;
	}
	
	pTimerCtx = si->si_value.sival_ptr;

	if (pTimerCtx != NULL) {
		pIApContext = pTimerCtx->pIApContext;
		curTime = iAP2TimeGetCurTimeMs();

	    //LOGD("==TIMER>> timerHandler() piAP2Timer[%p] sig[%d] si_signo[%d] curTime[%u]", pTimerCtx->piAP2Timer, sig, si->si_signo, curTime);
	    //LOGD("si_code[%d] sival_ptr[%p] context[%p]", si->si_code, si->si_value.sival_ptr, context);
		
	    pthread_mutex_lock(&pIApContext->iAP2LinkRLTMutex);

	   	// put the timer into timer queue.
        queuePut(pIApContext->pExpiredTimerQHead, pIApContext->pExpiredTimerQTail, pTimerCtx->piAP2Timer);

	    pthread_cond_signal(&pIApContext->iAP2LinkRLTCond);
	    pthread_mutex_unlock(&pIApContext->iAP2LinkRLTMutex);		
	}
	else
		LOGE("ERROR : pTimerCtx is NULL");
}

static void timerInit(int userSignal)
{
	int retValue;
	struct sigaction sa;

	LOGD("==TIMER== timerInit() userSignal[%d]", userSignal);

	sa.sa_flags 	= SA_SIGINFO;
	sa.sa_sigaction = timerHandler;
	sigemptyset(&sa.sa_mask);
	retValue = sigaction(userSignal, &sa, NULL);
	
    if (retValue != 0)
		LOGD("ERROR : errno[%d]: %s", errno, strerror (errno));

}
#else
static void signalHandler(int sig, siginfo_t *si, void *context)
{
	LOGD("sig[%d] si_code[%d] sival_ptr[%p] context[%p]", sig, si->si_code, si->si_value.sival_ptr, context);
}

static void timerHandler(union sigval val)
{
	IAPServiceContext_t* pIApContext = NULL;
	TimerContext_t* pTimerCtx = NULL;
	//uint32_t curTime = 0; //#LGE_CHANGE Fix warning about value which don't use.

	if (val.sival_ptr == NULL) {
		LOGE("ERROR : val.sival_ptr is NULL");
		return;
	}
	
	pTimerCtx = val.sival_ptr;

	if (pTimerCtx != NULL) {
		pIApContext = pTimerCtx->pIApContext;
		//curTime = iAP2TimeGetCurTimeMs(); //#LGE_CHANGE Fix warning about value which don't use.

//	    LOGD("==TIMER>> timerHandler() piAP2Timer[%p] curTime[%u]", pTimerCtx->piAP2Timer, curTime);
//	    LOGD("val.sigval_ptr[%p]", val.sival_ptr);
		
	    pthread_mutex_lock(&pIApContext->iAP2LinkRLTMutex);

	   	// put the timer into timer queue.
        queuePut(pIApContext->pExpiredTimerQHead, pIApContext->pExpiredTimerQTail, pTimerCtx->piAP2Timer);

	    pthread_cond_signal(&pIApContext->iAP2LinkRLTCond);
	    pthread_mutex_unlock(&pIApContext->iAP2LinkRLTMutex);		
	}
	else
		LOGE("ERROR : pTimerCtx is NULL");
}

static int timerCreate(struct sigevent *sigEvt, void *handle __attribute__((unused)), timer_t *timerId, void *ptr) //#LGE_CHANGE Fix warning which it don't use.
{
	int retValue = 0;

	sigEvt->sigev_notify            = SIGEV_THREAD;
	sigEvt->sigev_notify_function   = timerHandler;
	sigEvt->sigev_notify_attributes = NULL;
    sigEvt->sigev_signo             = SIGRTMIN;
	sigEvt->sigev_value.sival_ptr   = ptr;

	retValue = timer_create(CLOCK_REALTIME, sigEvt, timerId);
	if (retValue != 0) {
		LOGE("%s: timer_create fail, %s", __FUNCTION__, strerror(errno));
	}
	return retValue;
}

static void timerInit(int userSignal)
{
	int retValue;
	struct sigaction sa;

	LOGD("==TIMER== timerInit() userSignal[%d]", userSignal);

	sa.sa_flags 	= SA_SIGINFO;
	sa.sa_sigaction = signalHandler;
	sigemptyset(&sa.sa_mask);
	retValue = sigaction(userSignal, &sa, NULL);
	
    if (retValue != 0)
		LOGE("ERROR : errno[%d]: %s", errno, strerror (errno));

}
#endif

/*
****************************************************************
**
**  _iAP2TimeCleanupCallback
**
**  Input:
**      timer:      pointer to timer to cleanup
**
**  Output:
**      None
**
**  Return:
**      None
**
****************************************************************
*/
void _iAP2TimeCleanupCallback (iAP2Timer_t* timer)
{
	int retValue = 0;
	TimerContext_t* pTimerCtx = NULL;

	LOGD(" ==TIMER%% _iAP2TimeCleanupCallback() timer[%p]", timer);

	if (timer == NULL) {
		LOGE("ERROR : timer is null");
		return ;
	}

	pTimerCtx = timerListGetTimerContext(timer);

	if (pTimerCtx == NULL) {
		LOGE("ERROR : timer[%p] is not exist", timer);
		return;
	}

	LOGD("piAP2Timer[%p] delayMs[%u] callback[%p] pIApContext[%p]", pTimerCtx->piAP2Timer, pTimerCtx->delayMs, pTimerCtx->callback, pTimerCtx->pIApContext);
	LOGD("timerId[%d] siSigno[%d]", pTimerCtx->timerId, pTimerCtx->siSigno);

	retValue = timer_delete(pTimerCtx->timerId);
	if (retValue != 0) {
		LOGE("ERROR : errno[%d]: %s", errno, strerror (errno));
	}

    pthread_mutex_lock(&pTimerCtx->pIApContext->iAP2LinkRLTMutex);
    LOGD("timer : %p , ptimerCtx : %p", timer, pTimerCtx);
    if (listDeleteNodeWithData(pTimerCtx->pIApContext->pExpiredTimerQHead, pTimerCtx->pIApContext->pExpiredTimerQTail, timer) == FALSE)
        LOGE("ERROR : fail to delete node timer[%p]", timer);
    pthread_mutex_unlock(&pTimerCtx->pIApContext->iAP2LinkRLTMutex);

    pthread_mutex_lock(&pTimerCtx->pIApContext->iAP2LinkRLTMutex);
    if (listDeleteNodeWithData(pTimerCtx->pIApContext->pTimerListHead, pTimerCtx->pIApContext->pTimerListTail, pTimerCtx) == FALSE)
        LOGE("ERROR : fail to delete node pTimerCtx[%p]", pTimerCtx);
    pthread_mutex_unlock(&pTimerCtx->pIApContext->iAP2LinkRLTMutex);

	LOGD(" ==TIMER%% __iAP2TimeCleanupCallback() end== ");

}

/*
****************************************************************
**
**  _iAP2TimeCallbackAfter
**
**  Input:
**      timer:      timer
**      delayMs:    amount of time to wait before callback is called
**      callback:   function to callback on timeout
**
**  Output:
**      None
**
**  Return:
**      BOOL    return TRUE if successful in registering callback, else FALSE
**
**  Note:   This must be implemented by user of the library.
**          Call must return without blocking...
**          timer should be processed asynchronously.
**          Call to callback must occur either on the same thread
**          as the one processing link layer state machine.
**
****************************************************************
*/
BOOL _iAP2TimeCallbackAfter (iAP2Timer_t* timer,
                             uint32_t     delayMs,
                             iAP2TimeCB_t callback)
{
	int retValue;
	timer_t timerId;
	struct sigevent sigEvt;
	TimerContext_t* pTimerCtx = NULL;
	IAPServiceContext_t* pIApContext;
	BOOL bRetValue = FALSE;

    // for test // jungeyon.kim
    //return TRUE;

	// uint32_t curTime = iAP2TimeGetCurTimeMs(); //#LGE_CHANGE Fix warning about value which don't use.

//	LOGD("==TIMER++ _iAP2TimeCallbackAfter() timer[%p] delayMs[%u] callback[%p] curTime[%u]", timer, delayMs, callback, curTime);

    if (timer == NULL) {
		LOGE("ERROR : piAP2Timer is null");
		return FALSE;
	}

    pTimerCtx = timerListGetTimerContext(timer);
	if (pTimerCtx == NULL) {
		pTimerCtx = malloc(sizeof(TimerContext_t));
		if (pTimerCtx == NULL) {
			LOGE("ERROR : pTimerCtx is NULL");
			free(pTimerCtx);  // add code becauseof staticAnalisys (MemoryRick) Yongil.park Yongil.park 2015.04.22
			return FALSE;
		}
		memset(pTimerCtx, 0, sizeof(TimerContext_t)); // added memory init // soohwan.oh - 2015-01-16

		pIApContext = _getServiceContext(timer);
		if (pIApContext == NULL) {
			LOGE("ERROR : pIApContext is null");
			free(pTimerCtx); // add code becauseof staticAnalisys (MemoryRick) Yongil.park 2015.04.22
			return FALSE;
		}

#if (TIMER_NOTI == _SIGNAL)
		sigEvt.sigev_notify 		 = SIGEV_SIGNAL;
		sigEvt.sigev_signo			 = _timerGetEmptyUserSigal(pIApContext);//MY_TIMER_SIGNAL;
		sigEvt.sigev_value.sival_ptr = pTimerCtx;

		LOGD("MY_TIMER_SIGNAL[%d] pTimerCtx[%p] delayMs[%u]", MY_TIMER_SIGNAL, pTimerCtx, delayMs);
		retValue = timer_create(CLOCK_REALTIME, &sigEvt, &timerId);
#else
		memset(&sigEvt, 0, sizeof(sigEvt));
		retValue = timerCreate(&sigEvt, timer, &timerId, pTimerCtx);
#endif

		if (retValue != 0) {
			LOGE("ERROR : errno[%d]: %s", errno, strerror (errno));
		}
		else {
            timerInit(sigEvt.sigev_signo);
            pTimerCtx->piAP2Timer = timer;
			pTimerCtx->delayMs = delayMs;
			pTimerCtx->callback = callback;
			pTimerCtx->pIApContext = pIApContext;
			pTimerCtx->timerId = timerId;
			pTimerCtx->siSigno = sigEvt.sigev_signo; // MY_TIMER_SIGNAL;

			bRetValue = listInsertDataAfterNode(pIApContext->pTimerListHead, pIApContext->pTimerListTail, pTimerCtx, pIApContext->pTimerListHead);

			LOGD("Create New Timer Context bRetValue[%d] pTimerCtx[%p]", bRetValue, pTimerCtx);
			LOGD("piAP2Timer[%p] delayMs[%u] callback[%p] pIApContext[%p]", pTimerCtx->piAP2Timer, pTimerCtx->delayMs, pTimerCtx->callback, pTimerCtx->pIApContext);
			LOGD("timerId[%d] siSigno[%d]", pTimerCtx->timerId, pTimerCtx->siSigno);
		}
	}
	else {
//        LOGW("WARNING : timer[%p] is already exist", timer);
//		LOGD("piAP2Timer[%p] delayMs[%u] callback[%p] pIApContext[%p]", pTimerCtx->piAP2Timer, pTimerCtx->delayMs, pTimerCtx->callback, pTimerCtx->pIApContext);
//		LOGD("timerId[%d] siSigno[%d]", pTimerCtx->timerId, pTimerCtx->siSigno);
	}
	/* Set Timer Interval */
	//uint32_t nano_intv;
	struct itimerspec its;
	struct itimerspec oldIts;
	memset(&its, 0x00, sizeof(struct itimerspec));
	memset(&oldIts, 0x00, sizeof(struct itimerspec));

	// initial expiration
	its.it_value.tv_sec = delayMs / 1000;
	its.it_value.tv_nsec = (delayMs % 1000) * 1000000;

	//retValue = timer_settime(timerId, 0, &its, &oldIts);
	retValue = timer_settime(pTimerCtx->timerId, 0, &its, NULL); // soohwan.oh - 2015-01-17

	if (retValue != 0) {
		LOGE("ERROR : %d, %u %u errno[%d]: %s", 
				pTimerCtx->timerId, its.it_value.tv_sec, its.it_value.tv_nsec,
				errno, strerror (errno));
		return FALSE;
	}

	/* Save Timer Inforamtion */
	return TRUE;
}

/*
****************************************************************
**
**  _iAP2TimeCancelCallback
**
**  Input:
**      timer:      timer
**
**  Output:
**      None
**
**  Return:
**      None
**
**  Note:   This must be implemented by user of the library.
**
****************************************************************
*/
void _iAP2TimeCancelCallback (iAP2Timer_t* timer)
{
	int retValue = 0;
	TimerContext_t* pTimerCtx = NULL;

	LOGD("==TIMER-- _iAP2TimeCancelCallback() timer[%p]", timer);

	if (timer == NULL) {
		LOGE("ERROR : timer is null");
		return;
	}

	pTimerCtx = timerListGetTimerContext(timer);

	if (pTimerCtx == NULL) {
		LOGW("WARNING : timer[%p] is not exist", timer);
		return;
	}

    struct itimerspec its;
	struct itimerspec oldIts;
    memset(&its, 0x00, sizeof(struct itimerspec));
	memset(&oldIts, 0x00, sizeof(struct itimerspec));

//	LOGD("piAP2Timer[%p] delayMs[%u] callback[%p] pIApContext[%p]", pTimerCtx->piAP2Timer, pTimerCtx->delayMs, pTimerCtx->callback, pTimerCtx->pIApContext);
//	LOGD("timerId[%d] siSigno[%d]", pTimerCtx->timerId, pTimerCtx->siSigno);
	retValue = timer_settime(pTimerCtx->timerId, 0, &its, &oldIts);

	if (retValue != 0) {
		LOGE("ERROR : errno[%d]: %s", errno, strerror (errno));
	}	
}


