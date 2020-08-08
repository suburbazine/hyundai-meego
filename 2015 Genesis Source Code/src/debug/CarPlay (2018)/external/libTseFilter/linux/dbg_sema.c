/*
 *    @file        <Dbg_sema.c>
 *    @brief       semaphore API of LG Electronics Preprocessor
 *    @version     1.0.0
 *    @date        2015-02-11  1.0.0 initial version
 *    @author      speech team
 */

#include "dbg_sema.h"

/** @brief initialize an unnamed semaphore
 *
 *  @param sem semaphore object pointer.
 *  @param pshared 0 then the semaphore is shared between the threads of a process, others semaphore is shared between processes.
 *  @param value unused.
 *  @return 0 on success; on error, -1 is returned, and errno is set to indicate the error.
 *  @remark 주의사항 없음
 */
int DbgSemaInit( tDbgSema * sem, int pshared, unsigned value )
{
   int iError;

   iError = sem_init( sem, 0, 0);

   return iError;
}
/** @brief atomically decrements sem's count if it is greater than 0 and returns immediately or it suspends the calling thread until it can resume following a call to sem_post or sem_post_multiple.
 *
 *  @param sem semaphore object pointer.
 *  @return 0 for succes, other for errors.
 *  @remark 주의사항 없음
 */
int DbgSemaWait( tDbgSema * sem )
{
   int iError;

   iError = sem_wait( sem );

   return iError;
}
/** @brief either releases one thread if there are any waiting on sem, or it atomically increments sem's count.
 *
 *  @param sem semaphore object pointer.
 *  @return 0 for succes, other for errors.
 *  @remark 주의사항 없음
 */
int DbgSemaPost( tDbgSema * sem )
{
   int iError;

   iError = sem_post( sem );

   return iError;
}
/** @brief destroys a semaphore object, freeing the resources it might hold. No threads should be waiting on the semaphore at the time sem_destroy is called.
 *
 *  @param sem semaphore object pointer.
 *  @return 0 for succes, other for errors.
 *  @remark 주의사항 없음
 */
int DbgSemaDestroy( tDbgSema * sem )
{
   int iError;

   iError = sem_destroy( sem );

   return iError;
}
