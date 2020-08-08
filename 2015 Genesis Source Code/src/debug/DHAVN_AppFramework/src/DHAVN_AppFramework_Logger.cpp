/**
 * Logger.h
 *
 * Implementation of the Class Logger
 *
 * @author  Golyshkin A.
 * @CreatedDate 15-May-2011 11:20:51 AM
 *
 * Logger logger( Logger::LOG_CONSOLE );            
 *                                                                    
 * logger.Log( "Test Log" );                                        
 * logger.Log( "Test Log Low", Logger::PRIORITY_LOW );
 * logger.Log( "Test Log Medium", Logger::PRIORITY_MEDIUM );
 * logger.Log( "Test Log Hi", Logger::PRIORITY_LOW );
 * logger.Log( "Test Log Crt", Logger::PRIORITY_CRITICAL );
 *
 */
 
#include "DHAVN_AppFramework_Logger_Def.h"
#include <QDateTime>
#include <QTextStream>
#include <QtDebug>

class LoggerPrivate
{

private:

    /** Constructor */
    LoggerPrivate() : q_ptr(NULL){}

      
    /** Destructor */
    virtual ~LoggerPrivate() {}

public:


   //Q-Pointer to the AppEngineBase class
  Logger * const q_ptr;
  Q_DECLARE_PUBLIC(Logger)
};

#define DEFAULT_LOG_FILE_NAME "log.log"
const QString LOGGER_CONF_FILE = "/log_data/log/.hmi_logger";

Logger *Logger::m_pLogger = NULL;
QMutex Logger::m_instanceMutex;

Logger *
Logger::Instance()
{
   m_instanceMutex.lock();

   if ( !m_pLogger )
   {
      m_pLogger = new Logger("APP_NONE");
   }

   m_instanceMutex.unlock();
   return m_pLogger;
}

Logger *
Logger::Instance(const QString& inModuleName)
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
        m_pLogger = new Logger(inModuleName);
    }

   m_instanceMutex.unlock();
   return m_pLogger;
}

void
Logger::Destroy()
{
   m_instanceMutex.lock();
   
   // TODO: Print to check how many times this is called
   m_pLogger->Log(QString( "Logger Instance deleted "));
   delete m_pLogger;
   m_pLogger = NULL;
   m_instanceMutex.unlock();
}

Logger::Logger () : m_IsLoggerEnabled(false), d_ptr(NULL)
{
}

Logger::Logger(const QString inLogModule) :m_IsLoggerEnabled(true), d_ptr(NULL)
{
    LOG_INIT_V2(inLogModule);
    LOG_INFO<< "Logger::Logger " << inLogModule <<LOG_ENDL;

}

Logger::Logger( const Logger & aLogger)
    : QObject()
{
    m_IsLoggerEnabled = aLogger.m_IsLoggerEnabled;
    d_ptr = NULL;
}

Logger::~Logger()
{
}

void
Logger::CheckEnableLoggerSetting()
{
   if ( QFile::exists( LOGGER_CONF_FILE ) == true )
   {
      m_IsLoggerEnabled = true;
   }
}

void
Logger::SetLogType( LOG_TYPE_T aLogType, QString aModuleName )
{
    aLogType = aLogType;
    aModuleName = aModuleName;
    return;
}

void
Logger::Log( const QString &aMessage, LOG_PRIORITY aPriority )
{
    m_Mutex.lock();
    switch(aPriority)
    {
    case PRIORITY_LOW:
        LOG_LOW << aMessage << LOG_ENDL;
        break;
    case PRIORITY_MEDIUM:
        LOG_MEDIUM << aMessage << LOG_ENDL;
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

void
Logger::HandleSystemLogs( QtMsgType aType, const char *pMsg )
{
   Logger *pLogger = Logger::Instance();

   if ( pLogger->IsLoggerEnabled() )
   {
      switch ( aType )
      {
         case QtDebugMsg:
           pLogger->Log( pMsg, Logger::PRIORITY_LOW );
           break;
         case QtWarningMsg:
           pLogger->Log( pMsg, Logger::PRIORITY_MEDIUM );
           break;
         case QtCriticalMsg:
           pLogger->Log( pMsg, Logger::PRIORITY_HI );
           break;
         case QtFatalMsg:
           pLogger->Log( pMsg, Logger::PRIORITY_CRITICAL );
           break;
         default:
           break;
      }
   }
}


void 
Logger::Reserved_1()
{
}

void 
Logger::Reserved_2()
{
}

