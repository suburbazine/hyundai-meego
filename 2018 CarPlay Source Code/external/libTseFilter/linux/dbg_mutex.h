/*
 *    @file        <Dbg_mutex.c>
 *    @brief       mutex API header of LG Electronics Preprocessor
 *    @version     1.0.0
 *    @date        2015-02-11  1.0.0 initial version
 *    @author      speech team
 */

#if defined( __cplusplus )
extern "C"
{
#endif

#ifndef __DBG_MUTEX__
#define __DBG_MUTEX__

#include <pthread.h>

typedef pthread_mutex_t tDbgMutex;

int   DbgMutexInit( tDbgMutex* mutex );
int   DbgMutexDestroy( tDbgMutex* mutex );
int   DbgMutexLock( tDbgMutex* mutex );
int   DbgMutexUnlock( tDbgMutex* mutex );

#endif

#if defined( __cplusplus )
}
#endif
