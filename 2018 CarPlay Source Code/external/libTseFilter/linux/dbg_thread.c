/*
 *    @file        <Dbg_thread.c>
 *    @brief       thread API of LG Electronics Preprocessor
 *    @version     1.0.0
 *    @date        2015-02-11  1.0.0 initial version
 *    @author      speech team
 */

#include "dbg_thread.h"

/** @brief create and starts a new thread in the calling process
 *
 *  @param thread thread id.
 *  @param start_routine function pointer of start_routine().
 *  @param arg passed as the sole argument of start_routine().
 *  @return 0 for succes, other for errors.
 *  @remark 주의사항 없음
 */
int DbgThreadCreate( tDbgThread * thread,
                        void* (*start_routine)( void* ),
                        void* arg )
{
   int iError;

   iError = pthread_create( thread,
                            0,
                            start_routine,
                            arg );

   return iError;
}

/** @brief new thread terminates
 *
 *  @param thread thread id.
 *  @return 0 for succes, other for errors.
 *  @remark 주의사항 없음
 */
int DbgThreadJoin( tDbgThread thread )
{
   int iError;

   iError = pthread_join( thread, 0 );

   return iError;
}

// Not used
void DbgThreadDestroy( tDbgThread thread )
{

}

/** @brief thread sleep for a specified number of milliseconds
 *
 *  @param ms required sleep time.
 *  @return 0 for succes, other for errors.
 *  @remark 주의사항 없음
 */
int DbgThreadMilliSecSleep( unsigned int ms )
{
   usleep( ms * 1000 );

   return 0;
}

