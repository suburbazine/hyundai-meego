/*
 *    @file        <Dbg_sema.h>
 *    @brief       semaphore API header of LG Electronics Preprocessor
 *    @version     1.0.0
 *    @date        2015-02-11  1.0.0 initial version
 *    @author      speech team
 */

#if defined( __cplusplus )
extern "C"
{
#endif

#ifndef __DBG_SEMA__
#define __DBG_SEMA__

#include <semaphore.h>

typedef sem_t tDbgSema;

int DbgSemaInit( tDbgSema * sem, int pshared, unsigned value );
int DbgSemaWait( tDbgSema * sem );
int DbgSemaPost( tDbgSema * sem );
int DbgSemaDestroy( tDbgSema * sem );

#endif

#if defined( __cplusplus )
}
#endif
