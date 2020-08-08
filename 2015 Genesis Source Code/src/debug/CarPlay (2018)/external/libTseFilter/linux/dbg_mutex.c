/*
 *    @file        <Dbg_mutex.c>
 *    @brief       mutex API of LG Electronics Preprocessor
 *    @version     1.0.0
 *    @date        2015-02-11  1.0.0 initial version
 *    @author      speech team
 */
 
#include "dbg_mutex.h"

/** @brief initializes the mutex object pointed to by mutex according to the mutex attributes specified in mutexattr. If mutexattr is NULL, default attributes are used instead.
 *
 *  @param mutex mutex object pointer.
 *  @return 0 for succes, other for errors.
 *  @remark 주의사항 없음
 */
int DbgMutexInit( tDbgMutex* mutex )
{
   int iError;

   iError = pthread_mutex_init( mutex, 0 );

   return iError;
}

/** @brief initialize the mutex referenced by mutex with attributes specified by att.
 *
 *  @param mutex mutex object pointer.
 *  @return 0 for succes, other for errors.
 *  @remark 주의사항 없음
 */
int DbgMutexDestroy( tDbgMutex* mutex )
{
   int iError;

   iError = pthread_mutex_destroy( mutex );

   return iError;
}
/** @brief locks the given mutex. If the mutex is currently unlocked, it becomes locked and owned by the calling thread.
 *
 *  @param mutex mutex object pointer.
 *  @return 0 for succes, other for errors.
 *  @remark 주의사항 없음
 */
int DbgMutexLock( tDbgMutex* mutex )
{
   int iError;

   iError = pthread_mutex_lock( mutex );

   return iError;
}
/** @brief unlocks the given mutex. The mutex is assumed to be locked and owned by the calling thread on entrance to pthread_mutex_unlock.
 *
 *  @param mutex mutex object pointer.
 *  @return 0 for succes, other for errors.
 *  @remark 주의사항 없음
 */
int DbgMutexUnlock( tDbgMutex* mutex )
{
   int iError;

   iError = pthread_mutex_unlock( mutex );

   return iError;
}

