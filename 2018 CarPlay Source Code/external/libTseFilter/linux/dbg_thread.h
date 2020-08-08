/*
 *    @file        <Dbg_thread.h>
 *    @brief       thread API header of LG Electronics Preprocessor
 *    @version     1.0.0
 *    @date        2015-02-11  1.0.0 initial version
 *    @author      speech team
 */

#if defined( __cplusplus )
extern "C"
{
#endif

#ifndef __DBG_THREAD__
#define __DBG_THREAD__

#include <pthread.h>
#include <unistd.h>

typedef pthread_t tDbgThread;

int   DbgThreadCreate( tDbgThread* thread, void* (*start_routine)(void* ),void* arg );
int   DbgThreadJoin( tDbgThread thread );
void  DbgThreadDestroy( tDbgThread thread );

int   DbgThreadMilliSecSleep( unsigned int ms );

#endif

#if defined( __cplusplus )
}
#endif
