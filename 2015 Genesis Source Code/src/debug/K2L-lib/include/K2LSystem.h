/*=== K2L GmbH ===============================================================*/
/*
Target platform:    Linux V2.6.24
Language:           C++
*/
/**
\file
Definition of operating system abstraction classes.

\ingroup    K2LSystem

\par        COPYRIGHT (c) 2000-2009 by K2L GmbH
All rights reserved.

*/
/*============================================================================*/

#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <stdint.h>
#include <unistd.h>
#include <K2LSystemImportExport.h>
#include <pthread.h>
#include <sys/time.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h>

/*=============================================================================*/
/** \addtogroup K2LSystem
*/
/*=============================================================================*/
//Infinite timeout
#define INFINITE            0xFFFFFFFF

//@{

/**
The namespace K2L contains the definitions and classes of K2L GmbH.
*/
namespace k2l
{
    /**
    The namespace platform contains the platform abstraction.
    */
    namespace platform
    {
        /**
        The namespace system contains operating system abstraction.
        */
        namespace system
        {

            /**
            This function suspends the execution of the current thread for 
            a specified interval represented in miliseconds.
            */
            inline void ThreadSleep(uint32_t dwMilliseconds)
            {
                //TODO: Check ranges. Should be ok for most cases
                usleep(dwMilliseconds*1000);
            }

            /**
            This function delivers current system time in miliseconds.

            \note
            Please do not do any assumptions about time elapsed from system
            startup.

            \return
            System time in miliseconds.
            */
            inline uint32_t SystemGetTickCount(void)
            {
            	uint32_t milliseconds = 0;
            	static int32_t tickFactor = 0;
                tms tm;
                clock_t c = times(&tm);

                if (c != clock_t(-1))
                {
                	if (tickFactor <= 0)
                	{
						tickFactor = sysconf(_SC_CLK_TCK);

						if (tickFactor > 0 && tickFactor <= 1000000l)
						{
							tickFactor = 1000000l / tickFactor;
						}
                	}

                	if (tickFactor > 0)
					{
                		int64_t c64 = static_cast<int64_t>(c);
                		int64_t tickFactor64 = static_cast<int64_t>(tickFactor);
                		int64_t divisor64 = static_cast<int64_t>(1000);

						milliseconds = static_cast<uint32_t>((c64 * tickFactor64) / divisor64);
					}
                }

                return milliseconds;
            }


            /**
            Encapsulates a mutex/critical section like object, which is valid at least on process scope.
            Requirements: 
            -    Default constructor        :    Initialize the os-specific synchronization object
            -    Destructor                 :    Delete the os-specific synchronization object
            -    void Lock()                :    Block or grant exclusive access to the calling thread.
            -    void Unlock()              :    Release exclusive access. -> other threads may enter the critical section.
            -    No self inflicting deadlocks are allowed. This means: lock.Lock(); lock.Lock(); must NOT result in an 
            auto-deadlock of the executing thread.
            */
            class K2LPLATFORMSYSTEM_API CCriticalSection
            {
                pthread_mutex_t m_cs;
                pthread_mutexattr_t m_attr;
            public:

                CCriticalSection()
                    : m_cs()
                {
                    //TODO: Following two lines are needed to activate recursive variant of the mutex.
                    //Instead we use fast mutex. See the last line in constructor.
                    //Important: Per default fast mutex is used. Only drawback is that you can not
                    //lock two times in same thread. Second lock causes deadlock situation.
                    //pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_RECURSIVE_NP);
                    //pthread_mutex_init(&m_cs, &m_attr);

                    pthread_mutex_init(&m_cs, NULL);
                }

                virtual ~CCriticalSection()
                {
                    pthread_mutex_destroy(&m_cs);
                }

                void Lock()
                {
                    pthread_mutex_lock(&m_cs);
                }

                void Unlock()
                {
                    pthread_mutex_unlock(&m_cs);
                }
            };

            /**
            Template of an object, locking a resource at construction time and releasing it at destruction time.

            Requirements:
            -    Default constructor NOT accessible.
            -    Constructor( LockType& robject)    : Locks the object at construction time.
            -    Destructor                :    Unlocks the object at destruction time.
            */
            template <typename LockType>
            class CAutoLock
            {
                LockType & m_rLock;

            public:
                CAutoLock(LockType &rLock)
                    : m_rLock(rLock)
                {
                    m_rLock.Lock();
                }
                ~CAutoLock() throw ()
                {
                    m_rLock.Unlock();
                }
            };

            /**
            Definition of the default (workhorse) combination of CAutoLock and CCriticalSection.
            Usage example                    :    k2l::platform::system::CAutoLockCS lock( m_csFoo ); 
            */
            typedef CAutoLock<CCriticalSection > CAutoLockCS;

            /**
            Abstract base class, supplying the standard mechanism for active (threaded) objects.
            Requirements:
            -    Standard constructor    :    Initialize members to default state.
            -    Destructor                :    If object not stopped, attempt emergency stop. Issue warning in this case.
            -    bool Run()                :    Start the thread of the object. Return true, if it worked, false if it failed.
            ThreadPriority for the thread started: Application default TODO: Move some CThread features to CActiveObject.
            -    bool Stop()                :    Stop the thread of the object. Make sure it is stopped before the function returns.
            -    OnStartController()        :    Call this once at thread start time from objects thread.
            -    OnStopController()        :    Call this once, at thread shutdown time from the objects thread.
            -    OnExec()                :    Call this repeatedly, without delay from the objects thread until the thread is shut down.
            -    OnStopRequest()            :    Call this once from the Stop() function, if the caller of Stop() is not the objects thread.
            -    ThreadFunction            :    Do anything, required to make the thread interact smoothly with the environment of the target os.

            Win32 implementation calls CoInitializeEx( COINIT_MULTITHREADED) and CoUninitialize() to allow thread to participate in COM contexts.
            */
            class K2LPLATFORMSYSTEM_API CActiveObject
            {
                volatile bool m_bRun;
                CCriticalSection m_csObjectAccess;
                pthread_t m_hThread;
                //    pthread_attr_t m_threadAttribute;

            public:
                typedef pthread_t NativeThreadHandle_t;

                CActiveObject();
                virtual ~CActiveObject();

                /** 
                Starts the thread with specified priority and stack size.

                \return
                true indicates success, otherwise error occured.
                */
                bool Run(
                    int iPriority = GetThreadPriorityApplicationNormal(),
                    uint32_t dwStackSize = GetDefaultStackSize());

                /** 
                Stops current thread. Termination can be done out of the
                CActiveObject thread (created previously by Run()) context 
                or some other thread. In case that it is terminated out of the other
                thread following is done:
                1. OnStopRequest() is called to give possibility to unblock 
                the CActiveObject thred.
                2. Join with CActiveObject thread is done.

                \return
                true indicates success, otherwise error occured.
                */
                bool Stop();

                /**
                    This function is called before CActiveObject thread is started.
                    
                    \note
                    Function is called out of the CActiveObject context.
                */
                virtual void OnStartController() = 0;

                /**
                    This function is called before CActiveObject thread is terminated.
                    
                    \note
                    Function is called out of the CActiveObject context.
                */
                virtual void OnStopController() = 0;

                /**
                    This function represents the CActiveObject thread.
                    It is called cyclically untill CActiveObject thread is terminated.
                */
                virtual void OnExec() = 0;

                /**
                    This function is responsible for unblocking the CActiveObject 
                    thread. It is called in case that termination of the CActiveObject
                    thread is done out of the other thread context.
                */
                virtual void OnStopRequest();

                /// Returns the id of the active thread.
                virtual uint32_t GetActiveThreadId();

                /**
                    This function retrieves current thread id.
                    \return
                    Thread identifier.
                */
                static uint32_t GetCurrentThreadId();

                /**
                    This function retrieves current process id.
                    \return
                    Process identifier.
                */
                static uint32_t GetCurrentProcessId();

                /** 
                    Set the priority of target Active objects thread.
                    \return
                    true indicates success, otherwise error occured.
                */
                static bool SetThreadPriority(CActiveObject &target, int iPriority);

                /** 
                    Sets the priority of current thread.
                    \return
                    true indicates success, otherwise error occured.
                */
                static bool SetThreadPriority(int iPriority);

                /** 
                    Sets the priority of specified thread (through thread handle).
                    \return
                    true indicates success, otherwise error occured.
                */
                static bool SetThreadPriority(NativeThreadHandle_t hThread, int iPriority);

                // OS- specific thread priority categories.
                static int GetThreadPriorityMin();
                static int GetThreadPriorityMax();
                static int GetThreadPriorityIdle();
                static int GetThreadPriorityApplicationMin();
                static int GetThreadPriorityApplicationNormal();
                static int GetThreadPriorityApplicationMax();
                static int GetThreadPriorityRealtimeBelowDriversMin();
                static int GetThreadPriorityRealtimeBelowDriversMax();
                static int GetThreadPriorityDriverMin();
                static int GetThreadPriorityDriverMax();
                static int GetThreadPriorityRealtimeAboveDriversMin();
                static int GetThreadPriorityRealtimeAboveDriversMax();
                static uint32_t GetDefaultStackSize();

            private:
                static void* ThreadFunction(void * pvParameter);
                bool IsOwnThread();
            };


            /**
                A thread safe reference counter, depending on the platform either using a mutext/critical section (bad platforms)
                or atomic/interlocked operations. 
                The only relevant information, used by applications is, when the reference count returns to 0 - typically indicating
                that a reference counted object is not referenced anymore.
            */
            class K2LPLATFORMSYSTEM_API CReferenceCounter
            {
                // atomic.h keeps making trouble. Revert to a critical section...
                CCriticalSection m_csCounter;
                int32_t m_RefCount;
            public:

                CReferenceCounter(int32_t lInitialCount = 1L)
                    : m_RefCount(lInitialCount)
                {
                }

                inline void AddRef()
                {
                    CAutoLockCS lock(m_csCounter);
                    m_RefCount++;
                    //atomic_add(1,&m_RefCount);
                }

                inline bool Release()
                {
                    CAutoLockCS lock(m_csCounter);
                    m_RefCount--;
                    return 0 == m_RefCount;
                    //return 0 != atomic_sub_and_test(1,&m_RefCount);
                }
            };


            // Linux (POSIX) is a bit stupid, if it comes to events. They do not offer an adequate system/kernel object, allowing to have WaitForMultiple() semantics.
            // This is why the following implementation of CEvent is the most complex one compared to any other OS.
            // It is also coming with the biggest risk of failure, if not exhaustively tested and reviewed.
            // Some extra types and mechanisms have to be implemented to make the thing work, transparent to applications.
            // We have 2 use cases of event objects:
            // 1. WaitForSingleObject() with a timeout.
            // 2. WaitForMultipleObjects() with a timeout. This is the Linux porting worst case.
            // The chosen strategy might be correct but it is also expensive in terms of lock/unlock operations.
            // Do not try to "simplify/optimize" without 2 days of meditation on the problem!

            // Linux special implementation. (Slow but might work). Internet sources propose to have 1 extra thread per wait condition, which is even worse.
            // We do not buy into this. Instead, we wish to keep the number of threads constant to other ports of the applications.
            // The chosen approach uses a scheme which works to up to 32 events, participating in the "wait any" game.
            // We have a delta signaling context instance, which may be manipulated by the individual, participating event objects.
            // In the first step, we make sure, none of the participating event objects is able to change its state. We check if any is signaled.
            // If non is currently signaled, we hook them to our delta signaling instance. - No races are possible so far, since the participating
            // events are still locked.
            // Then we unlock all hooked, participating event objects. Other threads may access our delta signaling context variable now even
            // while we are about to unlock the remaining participating events.
            // Then we acquire the main mutex for our main condition variable.
            // We test, if any signaling has occured before we block on cond_timedwait() or cond_wait(). If still no signaling, we
            // enter wait.
            // For the scheme to work, the delta signaling context has to use our mutex as well if it is receiving a signal of a participating event.
            // Would we forget this, a race condition would exist.

            struct IEventStateListener;

            /**
                This class abstracts "Event" operating system concept.
            */
            class K2LPLATFORMSYSTEM_API CManualResetEvent
            {
            private:
                volatile bool             m_bSignaled;
                volatile bool            m_bCanceled; // TODO: remove??
                pthread_mutex_t         m_mtxAccess;
                pthread_cond_t             m_condSignal;
                IEventStateListener *     m_pListener;
                uint32_t                 m_cookie;

                bool SetListener( uint32_t cookie, IEventStateListener *pListener );
                void RemoveListener();
                bool IsSignaled() const;
                void Lock();
                void Unlock();

                friend class CMultiEventListener;

            public:

                typedef CManualResetEvent * NativeEventType;

                explicit CManualResetEvent(bool bCreate = false);
                virtual ~CManualResetEvent();

                bool Create();

                void Close();

                void Set();

                void Reset();

                bool Wait(uint32_t dwTimeout = INFINITE);

                operator NativeEventType()
                {
                    return this;
                }

                static
                    inline
                    void
                    NativeSet
                    (NativeEventType nativeEvent
                    )
                {
                    nativeEvent->Set();
                }

                static
                    inline
                    void
                    NativeReset
                    (NativeEventType nativeEvent
                    )
                {
                    nativeEvent->Reset();
                }


                static
                    bool
                    NativeWaitAny
                    (NativeEventType *pEvents
                    , uint32_t dwCount
                    , uint32_t &rIndexActiveEvent
                    , uint32_t dwTimeOut = INFINITE
                    );
            };

            /**
                Implements periodic timer that triggers user event in specified cycle.
            */
            class K2LPLATFORMSYSTEM_API CPeriodicTimer
                : private CActiveObject
            {
            protected:
                
                CManualResetEvent *        m_pEvTarget;
                CManualResetEvent        m_evShutdown;
                uint32_t            m_CycleTime;
                bool m_active;

            public:
                CPeriodicTimer()
                    : m_pEvTarget(NULL)
                    , m_evShutdown(true)
                    , m_CycleTime(10)
                {
                }
                virtual ~CPeriodicTimer()
                {
                }

            public:
                /**
                    Start periodic timer with specified cylce time.
                    Event will be set periodically in this cycle.
                */
                void Start( CManualResetEvent *pTarget, uint32_t dwCycleTime, int priority )
                {
                    m_pEvTarget = pTarget;
                    if( NULL != m_pEvTarget )
                    {
                        m_CycleTime = dwCycleTime;
                        m_evShutdown.Reset();
                        Run( priority );
                    }
                }

                /**
                    This function stops periodic timer.
                */
                void Stop()
                {
                    CActiveObject::Stop();
                }

            private:
                virtual void OnStartController()
                {
                }

                virtual void OnStopController()
                {
                }

                virtual void OnExec()
                {
                    if( m_evShutdown.Wait( m_CycleTime ) )
                    { // Shutdown requested. Thats it.
                    }
                    else
                    {
                        if(NULL != m_pEvTarget )
                        {
                            m_pEvTarget->Set();
                        }
                    }
                }

                virtual void OnStopRequest()
                {
                    m_evShutdown.Set();
                }
            };

            /**
                Defines as set of static member function, which are reentrant and do the stuff, typically required
                from a runtime library - like memset, memcopy etc.
                This has been added because the smalles target platforms might not even provide a standard c-runtime
                library. Some of the c-runtime library functions are not very well designed, too. Thus, the version
                of those functions found here, provide "added value".
                Note: By design, this class does NOT define any memory management functions (malloc etc.).
            */
            class K2LPLATFORMSYSTEM_API CRuntime
            {
            public:
                /**
                    memcpy() semantics. No discussion about "overlapped memory ranges" please!
                */
                static inline void MemoryCopy(
                    void *pvTargetBuffer,
                    uint32_t targetBufferCapacity,
                    const void* cpvSource,
                    uint32_t numberOfBytesToCopy)
                {
                    ::memcpy(pvTargetBuffer, cpvSource, numberOfBytesToCopy);
                }
                /** 
                    For those who think they need it. memmove() semantics.
                */
                static inline void MemoryMove(
                    void *pvTargetBuffer,
                    uint32_t targetBufferCapacity,
                    const void* cpvSource,
                    uint32_t numberOfBytesToCopy)
                {
                    ::memmove(pvTargetBuffer, cpvSource, numberOfBytesToCopy);
                }

                /**
                    Set target buffer with specified value.
                */
                static inline void MemorySet(
                    void *pvTargetBuffer,
                    uint8_t value,
                    uint32_t count)
                {
                    ::memset(pvTargetBuffer, value, count);
                }

                /**
                    Clear target buffer - Set memory to all zeroes.
                */
                static inline void MemoryClear(void *pvTargetBuffer, uint32_t count)
                {
                    ::memset(pvTargetBuffer, 0x00, count);
                }

                /**
                    Check if two memory buffers are same.
                    \return
                    true if buffers are identical, otherwise false.
                */
                static inline bool MemoryEqual(
                    const void* cpBuf1,
                    const void* cpBuf2,
                    uint32_t count)
                {
                    return (memcmp(cpBuf1, cpBuf2, count) == 0);
                }
            };

            /**
                Implements memory management functions with added value that isolated and limited
                heap area can be allocated.
            */
            class K2LPLATFORMSYSTEM_API CMemoryManagement
            {
            public:

                enum
                {
                    HEAP_CAPACITY_GROWABLE = 0
                };


            	/**
                    Defines type that represents handle of the heap.
                */
                typedef void* HeapHandle_t;

                /** 
                    This function creates heap with specified initialSize and maximumSize in bytes.
                    \return
                    - handle to the newly created heap indicates success. 
                    - NULL indicates failure. 
                */
                static HeapHandle_t Create(uint32_t initialSize, uint32_t maximumSize);

                /**
                    This function allocates memory from previously created heap.
                    \return
                    Valid pointer not equal NULL indicates success.
                */
                static void* Alloc(HeapHandle_t handle, uint32_t bytes);

                /**
                    This function frees memory previously allocated by \ref Alloc() function.
                    \return
                    - true - Success
                    - false - Failure
                */
                static bool Free(HeapHandle_t handle, void* pMemory);

                /**
                    This function destroys heap previously created by \ref Create() function.
                    \return
                    - true - Success
                    - false - Failure
                */
                static bool Destroy(HeapHandle_t handle);
            };
        }
    }
}

//@}

#endif

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

