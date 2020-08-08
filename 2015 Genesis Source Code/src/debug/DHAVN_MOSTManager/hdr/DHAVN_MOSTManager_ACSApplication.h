#ifndef __EASYACS_H
#define __EASYACS_H



#include <QMutex>
#include <queue>
#include <vector>
#include <stdint.h>

#include <K2LSystem.h>
#include <ACSStreams.h>
#include <ACSMessage.h>
#include <K2LACSClient.h>
#include <K2LACSMOST.h>
#include <K2LACSEvents.h>

#include <DHAVN_LogUtil.h>

#include "DHAVN_MOSTManager_LogWrapper.h"


// This interface needs to be implemented by the application.
// It gives the status of pending messages, waiting to be marshaled
// into the applications main thread.
// MessagesAvailable() is called in the context of another thread
// NoMessagesAvailable() is called in the context of the application thread.
// Both functions can be used to Set/Reset a manual reset event or
// in case of a Win32 GUI application, MessagesAvailable() can be used for a
// ::PostMessage() call and NoMessagesAvailable() can be left empty.

class CIACSApplicationEvents
{
        public:
            CIACSApplicationEvents() {}
            virtual ~CIACSApplicationEvents() {}

            virtual void MessagesAvailable() = 0;
            virtual void NoMessagesAvailable() = 0;
};


// A purely technical base class which allows all facets of application classes to be lockable.

class CLockable
{

    k2l::acs::client::IMessageDispatcher * m_pDispatcher;

    // Make sure the compiler does not use implicitely generated functions.
    CLockable( const CLockable& crOther ); // Not available
    const CLockable& operator= ( const CLockable& crOther ); // Not available

    CLockable(); // Not available

public:
    explicit CLockable(k2l::acs::client::IMessageDispatcher * pDispatcher)
        : m_pDispatcher(pDispatcher)
    {

    }
    ~CLockable() { }

    void Lock(){ m_pDispatcher->Lock(); }

    void Unlock(){ m_pDispatcher->Unlock(); }

};

typedef k2l::platform::system::CAutoLock<CLockable > CACSLock;



// One instance of this class uses exactly one connection to the server.
// All MOST elements and event configurations are associated to that
// single connection.
// This implementation allows to marshal messages to another thread, for example the
// GUI or main thread of the application.

class CMarshaledACSApplication
                : public CLockable
                , public k2l::acs::client::IMessageDispatcher
                , public k2l::acs::client::IMessageListener
{
        typedef std::queue<k2l::acs::base::CACSMessage *> CMessageQueue;
        typedef std::queue<uint32_t> CProtocolTypeQueue;

        typedef std::vector<k2l::acs::client::IMessageListener *> CProtocols;

        k2l::acs::client::CACSClient m_client;
        k2l::acs::client::CActiveMessageDispatcher m_activeDispatcher;
        k2l::acs::protocols::events::CACSEventConnector m_eventConnector;
        k2l::acs::protocols::most::CMOSTElements m_MOSTElements;

        CProtocols m_protocols;

        k2l::acs::client::CPassiveMessageDispatcher::Cookie_t m_cookie;

        // The marshaling message queue. This is required here since we want to run
        // the rest of the framework in the main application thread, which could very well
        // be a Win32 GUI application which imposes the requirement to run "singlethreaded".
        // Yet, this scheme can also be useful if a single application thread has to process
        // further asynchronous activities such as a communication with another device such as
        // a tuner module, connected via I2C...or whatever...
        k2l::platform::system::CCriticalSection m_queueLock;
        CMessageQueue m_messageQueue;
        CProtocolTypeQueue m_protocolQueue;

        CIACSApplicationEvents *m_pACSApplicationEventListener;

        // Used for application-framework-dispatcher-access synchronization.
        k2l::platform::system::CCriticalSection m_dispatcherLock;
public:
        explicit CMarshaledACSApplication(CIACSApplicationEvents *pListener)
					: CLockable(this) // The warning you see at compile time tells you that you must never do this if you do more to the pointer than store it in a member variable.
					, m_client()
					, m_activeDispatcher()
					, m_eventConnector()
					, m_MOSTElements()
					, m_protocols()
					, m_cookie(0)
					, m_messageQueue()
					, m_protocolQueue()
					, m_pACSApplicationEventListener(pListener)
					, m_dispatcherLock()
        {                
                m_cookie = m_activeDispatcher.AddListener( this );
                m_eventConnector.Dispatcher( this );
                m_MOSTElements.Dispatcher( this );
        }

        void AddProtocol( k2l::acs::client::IMessageListener *pProtocol )
        {
                pProtocol->Dispatcher( this );
                m_protocols.push_back( pProtocol );
        }
				

        k2l::acs::protocols::events::CACSEventConnector *Events()
        {
                return &m_eventConnector;
        }

        k2l::acs::protocols::most::CMOSTElements *MOST()
        {
                return &m_MOSTElements;
        }

        bool StartApplication( int32_t threadPriority, uint32_t busAddress )
        {
                if( m_client.ConnectApplicationToBus( "", 5287, busAddress ) )
                {
                        m_activeDispatcher.SetClient( &m_client );
                        if( m_activeDispatcher.Run( threadPriority ) )
                        {
                                return true;
                        }
                }
                return false;
        }

        bool StartApplication(
                   int32_t threadPriority,
                   const char* serverAddress,
                   int serverPort,
                   uint32_t busAddress )
        {
                if( m_client.ConnectApplicationToBus( serverAddress, serverPort, busAddress ) )
                {
                        m_activeDispatcher.SetClient( &m_client );
                        if( m_activeDispatcher.Run( threadPriority ) )
                        {
                                return true;
                        }
                }
                return false;
        }

        bool StopApplication()
        {
                if( m_activeDispatcher.Stop() )
                {
                        m_client.Disconnect();
                        return true;

                }
                return false;
        }

        // IMessageDispatcher implementation
public:
        virtual uint32_t LocalNodeAddress() const
        {
                return m_activeDispatcher.LocalNodeAddress();
        }

        virtual void SetClient( k2l::acs::client::CACSClient *pClient )
        {
                // EMPTY - we use our own client. Do not call.

        }

        virtual k2l::acs::client::CACSClient * GetClient()
        {
                return m_activeDispatcher.GetClient();
        }


        virtual Cookie_t AddListener( IMessageListener * pListener)
        {
                // Not implemented. Do not call. Not required either, since we already have our connectors
                // wired.
                return 0;
        }

        virtual void RemoveListener( Cookie_t cookie)
        {
                // Not implemented. Do not call. Not required either, since we already have our connectors
                // wired.
        }

        // Does the dispatching of 1 incoming message.
        // Passive dispatchers need repeated calls of this method by the application.
        // Active dispatchers have a dedicated thread to call this method.
        virtual bool Dispatch()
        {
                bool dispatched = false;
                // Remark: This locking scheme is simple and secure and works fine and
                // if the handler code is slow, we leave the messages on server side.
                k2l::platform::system::CAutoLockCS lock( m_queueLock );
                if( !m_messageQueue.empty() )
                {
                        dispatched = true;
                        k2l::acs::base::CACSMessage *pMessage = m_messageQueue.front();
                        uint32_t protocol = m_protocolQueue.front();
                        {
                                k2l::platform::system::CAutoLockCS lock( m_dispatcherLock );
                                m_MOSTElements.OnMessage( protocol, pMessage );
                                m_eventConnector.OnMessage( protocol, pMessage );

                                for( CProtocols::iterator iter = m_protocols.begin(); iter != m_protocols.end(); iter++ )
                                {
                                        (*iter)->OnMessage( protocol, pMessage );
                                }
                        }

                        m_messageQueue.pop();
                        m_protocolQueue.pop();
                        pMessage->Release();
                }
                else
                {
                        m_pACSApplicationEventListener->NoMessagesAvailable();
                }

                return dispatched;
        }

        // Run/Stop: Active dispatchers need it and passive dispatchers will ignore Run() and return false.
        virtual bool Run( int32_t priority)
        {
                return false;
        }

        virtual bool Stop()
        {
                return false;
        }


        // Use this function if you fully configured the message to be sent (the first and all other segments must be correct).
        virtual bool Send( k2l::acs::base::CACSMessage *pMessage)
        {
                return m_activeDispatcher.Send( pMessage );
        }

        // Use this function to send if you did not create the first segment of the message and if you want to send the message
        // to the local node (which is the most common use case). This function will create the first segment for you.
        virtual bool Send( uint32_t protocol, k2l::acs::base::CACSMessage *pMessage)
        {
                return m_activeDispatcher.Send(protocol, pMessage );
        }

        // This version of Send is currently NOT used but it will be useful for future protocols.
        // It allows to directly send a message to a node which is not the local node.
        virtual bool Send( uint32_t protocol, uint32_t targetNodeAddress
                                        , k2l::acs::base::CACSMessage *pMessage )
        {
            return m_activeDispatcher.Send(protocol, targetNodeAddress, pMessage );
        }

        virtual void Shutdown()
        {
                m_activeDispatcher.Shutdown();
        }

        virtual void Lock()
        {
                m_dispatcherLock.Lock();
        }

        virtual void Unlock()
        {
                m_dispatcherLock.Unlock();
        }

        // IMessageListener implementation
public:

        virtual void Dispatcher( IMessageDispatcher *pDispatcher)
        {
                // Not implemented, not required here. Do not call.
            return;
        }

        virtual IMessageDispatcher * Dispatcher()
        {
                return &m_activeDispatcher;
        }

        virtual void OnConnect()
        {
                k2l::platform::system::CAutoLockCS lock( m_dispatcherLock );
                m_eventConnector.OnConnect();
                m_MOSTElements.OnConnect();
                for( CProtocols::iterator iter = m_protocols.begin(); iter != m_protocols.end(); iter++ )
                {
                        (*iter)->OnConnect();
                }
        }

        virtual void OnDisconnect()
        {
                k2l::platform::system::CAutoLockCS lock( m_dispatcherLock );
                m_eventConnector.OnDisconnect();
                m_MOSTElements.OnDisconnect();
                for( CProtocols::iterator iter = m_protocols.begin(); iter != m_protocols.end(); iter++ )
                {
                        (*iter)->OnDisconnect();
                }
        }


        virtual void OnMessage( uint32_t protocol, k2l::acs::base::CACSMessage *pMessage)
        {
                {
                        k2l::platform::system::CAutoLockCS lock( m_queueLock );
                        m_messageQueue.push( pMessage );
                        pMessage->AddRef();
                        m_protocolQueue.push( protocol );
                        m_pACSApplicationEventListener->MessagesAvailable();
                }
        }
};

class CControlThreadBase
{
    const char *LOG_TAG_ID;

protected:
    USE_LOG_UTIL

    /** Saves context to application to communicate to ACS daemon */
    CMarshaledACSApplication *m_AppContext;
    k2l::acs::protocols::most::IMOSTElement *m_pMOSTElement;

    bool m_Initialized;
    bool m_Registered;
    QMutex m_LogMutex;

public:
    CControlThreadBase(CMarshaledACSApplication *context, const char *log_tag)
        : LOG_TAG_ID(log_tag),
          m_AppContext(context),
          m_pMOSTElement(0),
          m_Initialized(false),          
          m_Registered(false)
    {
    }
    ~CControlThreadBase(){}

    bool GetIsRegistered() { return m_Registered; };

    virtual bool Initialize()
    {
        bool result = (m_pMOSTElement->InitializeMOSTElement()) ? true : false;
        return result;
    }

    virtual bool Register()
    {
        bool result = false;

        MOST_LOG_SIGNAL << LOG_TAG_ID << " Register API got called " << MOST_LOG_ENDL;

        if(!m_Registered)
        {
            if(m_pMOSTElement->IsFunctionBlock())
            {
                result = m_AppContext->MOST()->AddFunctionBlock(m_pMOSTElement);
            }
            else
            {
                result = m_AppContext->MOST()->AddShadow(m_pMOSTElement);
            }

            if(result)
            {
                MOST_LOG_INFO << LOG_TAG_ID << " Register FBlock successfully " << MOST_LOG_ENDL;
                m_Registered = true;
            }
            else
            {                
                MOST_LOG_CRITICAL << LOG_TAG_ID << " Failed to Register FBlock " << MOST_LOG_ENDL;
            }
        }
        else
        {
            // Registered already
            MOST_LOG_INFO << LOG_TAG_ID << " FBlock was already registered " << MOST_LOG_ENDL;
            result = true;
        }

        return result;
    }

    virtual bool UnRegister()
    {
        bool result = false;

        MOST_LOG_SIGNAL << LOG_TAG_ID << " UnRegister API got called " << MOST_LOG_ENDL;

        if(m_Registered)
        {
            if(m_pMOSTElement->IsFunctionBlock())
            {
                result = m_AppContext->MOST()->RemoveFunctionBlock(m_pMOSTElement);
            }
            else
            {
                result = m_AppContext->MOST()->RemoveShadow(m_pMOSTElement);
            }

            if(result)
            {
                MOST_LOG_INFO << LOG_TAG_ID << " UnRegister FBlock successfully " << MOST_LOG_ENDL;
                m_Registered = false;
            }
            else
            {
                MOST_LOG_CRITICAL << LOG_TAG_ID << " Failed to Unregister FBlock " << MOST_LOG_ENDL;
            }
        }
        else
        {
            // Unregistered already
            MOST_LOG_INFO << LOG_TAG_ID << " FBlock was already Unregistered " << MOST_LOG_ENDL;
            result = true;
        }

        return result;
    }
};


#endif
