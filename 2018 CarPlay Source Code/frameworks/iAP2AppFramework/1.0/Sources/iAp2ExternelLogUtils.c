/*
    File:       iAp2ExternelLogUtils.c
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


/* LGE_CHANGE_S, [CarPlay], jaeheon.kim@lge.com, 2015-07-10
 * To select log print to Log Assistance or not.
 * Log Assistance is IVI AVN2 team's log service.
 * If you use this block, Set IVIAVN2_DH_LOG in $(SRCROOT)/PlatformPOSIX/Makefile.
 * Log Assistance use mutex, but all application use Log Assistance. So, mutex lock problem is big issue.
 * So, CarPlay use another thread for wait mutex lock.
 */
#include "iAp2ExternelLogUtils.h"
#if defined(MEEGO)
#include "LogAssistance.h"
#elif defined(MIBGP)
#include "bslog.h"
#endif
#include <sys/queue.h>
#include <pthread.h>

struct tailq_entry {
  LogLevel level;
  char * logStr;
  TAILQ_ENTRY(tailq_entry) entries;
};

TAILQ_HEAD(, tailq_entry) my_tailq_head_iAP2;

static pthread_t             gLogWriteThread = 0;
static pthread_mutex_t         gLogQueueMutex;
static pthread_mutex_t *    gLogQueueMutexPtr;


void iAp2ExternelLogWriteAddQueue(LogLevel inLevel, const char *inStr, int inLen) {
    struct tailq_entry * item = NULL;

    // Tail queue initialize
    if (TAILQ_EMPTY(&my_tailq_head_iAP2)) TAILQ_INIT(&my_tailq_head_iAP2);

    if (gLogWriteThread != 0) {
        // Add log string to queue
        // 1. make log string
        item = (struct tailq_entry *)malloc(sizeof(*item));
        memset(item, 0x00, sizeof(*item));
        if( item != NULL ) {
            item->level = inLevel;
            item->logStr = (char*)malloc(inLen+1);
            if( item->logStr != NULL) {
                memcpy(item->logStr, inStr, inLen);
                item->logStr[inLen] = '\0';
                // 2. insert to queue
                pthread_mutex_lock(gLogQueueMutexPtr);
                TAILQ_INSERT_TAIL(&my_tailq_head_iAP2, item, entries);
                pthread_mutex_unlock(gLogQueueMutexPtr);
            }
            else {
                LOGE("ERROR malloc failed!!  item->logStr is null");
            }
        }
        else {
            LOGE("ERROR malloc failed!!  item is null");
        }
    }
}

static void * __iAp2ExternelLogWriteThread( void * inArgs ) {
    (void) inArgs;
    struct tailq_entry * item = NULL;

    while(gLogWriteThread != 0) {
        // Print log to Log Assistance
        while (TAILQ_FIRST(&my_tailq_head_iAP2) != NULL) {
            item = TAILQ_FIRST(&my_tailq_head_iAP2);

#if defined(MEEGO)
            if( item->level >= kLogLevelInfo )
                LOGA_HIGH( "%s", item->logStr );
            else
                LOGA_LOW( "%s", item->logStr );
#elif defined(MIBGP)
            if (item->level > kLogLevelWarning ) {
                // Error
                lg_print(eLGDP_IAP2_ERROR, "%s", item->logStr );
            } else if (item->level > kLogLevelNotice ) {
                // Warning
                lg_print(eLGDP_IAP2_WARNING, "%s", item->logStr );
            } else if (item->level > kLogLevelInfo ) {
                // Debug
                lg_print(eLGDP_IAP2_DEBUG, "%s", item->logStr );
            } else {
                // Info
                lg_print(eLGDP_IAP2_INFO, "%s", item->logStr );
            }
#endif
            pthread_mutex_lock(gLogQueueMutexPtr);
            TAILQ_REMOVE(&my_tailq_head_iAP2, item, entries);
            pthread_mutex_unlock(gLogQueueMutexPtr);
            if (item->logStr != NULL) {
                free(item->logStr);
            }

            if ( item != NULL ) {        
                free(item);
            }
        }
        usleep(100*1000);
    }

    pthread_exit(NULL);

    return (NULL);
}

OSStatus iAp2ExternelLogWriteInit(void)
{
    OSStatus err = 0;

    // Thread create and mutex initialize
    if( gLogWriteThread == 0 ) {
        err = pthread_mutex_init(&gLogQueueMutex, NULL);
        if( err != 0) {
            goto exit;
        }
        gLogQueueMutexPtr = &gLogQueueMutex;

        err = pthread_create(&gLogWriteThread, NULL, __iAp2ExternelLogWriteThread, NULL);
        if( err != 0) {
            goto exit;
        }        
    }
    // Tail queue initialize
    TAILQ_INIT(&my_tailq_head_iAP2);

exit:
    if( err ) {
        gLogWriteThread = 0;
    }

    return err;
}

void iAp2ExternelLogWriteStop(void)
{
    // Thread force exit.
    if (gLogWriteThread != 0) {
        pthread_cancel(gLogWriteThread);
        gLogWriteThread = 0;
    }

    // Mutex destroy
    if (gLogQueueMutexPtr != NULL) {
        pthread_mutex_destroy(gLogQueueMutexPtr);
        gLogQueueMutexPtr = NULL;
    }

    // Free the entire tail queue.
    while (TAILQ_FIRST(&my_tailq_head_iAP2) != NULL) {
        struct tailq_entry * item = TAILQ_FIRST(&my_tailq_head_iAP2);
        TAILQ_REMOVE(&my_tailq_head_iAP2, item, entries);
        if (item->logStr != NULL) {
            free(item->logStr);
        }

        if ( item != NULL ) {        
            free(item);
        }
    }
}

/* LGE_CHANGE_E, [CarPlay] */

