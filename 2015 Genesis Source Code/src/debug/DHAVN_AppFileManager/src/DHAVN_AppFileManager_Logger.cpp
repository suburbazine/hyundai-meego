#include "DHAVN_AppFileManager_Logger.h"
#include "DHAVN_AppFramework_Logger_Def.h"
#include <QDateTime>
#include <QTextStream>
#include <QtDebug>

LoggerEx *LoggerEx::m_pLogger = NULL;
QMutex LoggerEx::m_instanceMutex;

LoggerEx *
LoggerEx::Instance()
{
   m_instanceMutex.lock();

   if ( !m_pLogger )
   {
       m_pLogger = new LoggerEx("AppFileManager");
   }

   m_instanceMutex.unlock();
   return m_pLogger;
}

LoggerEx *
LoggerEx::Instance(const QString& inModuleName)
{
    m_instanceMutex.lock();

    static int m_firstTime = 0;

    /** since ParseAppArguments instantiates logger without modulename,
      *the first few logs of each application is logged with
      * module name as APP_NONE.
      * When applications instantiate logger with ModuleName, the following
      * code will ensure that old instance is deleted, and Module name is saved. */
    if(m_firstTime == 0)
    {
        m_firstTime = 1;
        if(m_pLogger)
        {
            delete m_pLogger;
            m_pLogger = NULL;
        }
    }
    if ( !m_pLogger )
    {
        m_pLogger = new LoggerEx(inModuleName);
    }

   m_instanceMutex.unlock();
   return m_pLogger;
}

void
LoggerEx::Destroy()
{
   m_instanceMutex.lock();

   // TODO: Print to check how many times this is called
   delete m_pLogger;
   m_pLogger = NULL;
   m_instanceMutex.unlock();
}

LoggerEx::LoggerEx()
    :m_priority(PRIORITY_LOW)
{
    m_Mutex.lock();
    m_logString.clear();
    m_stream.setString(&m_logString);
    m_Mutex.unlock();

}

LoggerEx::LoggerEx(const QString inLogModule)
:m_priority(PRIORITY_LOW)
{
    m_Mutex.lock();
    m_logString.clear();
    m_stream.setString(&m_logString);
    m_Mutex.unlock();

    LOG_INIT_V2(inLogModule);
    LOG_INFO<< "Logger::Logger " << inLogModule <<LOG_ENDL;

}

LoggerEx::~LoggerEx()
{
}

void
LoggerEx::Log( const QString &aMessage, LOG_PRIORITY aPriority )
{
    m_Mutex.lock();
    switch(aPriority)
    {
    case PRIORITY_LOW:
        LOG_LOW << aMessage << LOG_ENDL;
        break;
    case PRIORITY_MEDIUM:
#ifndef DISABLE_MP_MEDIUM_LOG
        LOG_MEDIUM << aMessage << LOG_ENDL;
#endif
        break;
    case PRIORITY_HI:
        LOG_HIGH << aMessage << LOG_ENDL;
        break;
    case PRIORITY_CRITICAL:
        LOG_CRITICAL << aMessage << LOG_ENDL;
        break;
     case PRIORITY_ASSERT:
        LOG_CRITICAL << aMessage << LOG_ENDL;
        break;
    case PRIORITY_TRACE:
        LOG_TRACE << aMessage << LOG_ENDL;
        break;
    case PRIORITY_INFO:
        LOG_INFO << aMessage << LOG_ENDL;
        break;
    case PRIORITY_SIGNAL:
        LOG_SIGNAL << aMessage << LOG_ENDL;
        break;
    case PRIORITY_SLOT:
        LOG_SLOT << aMessage << LOG_ENDL;
        break;
    case PRIORITY_TRANSITION:
        LOG_TRANSITION << aMessage << LOG_ENDL;
        break;
    }
    m_Mutex.unlock();
}

