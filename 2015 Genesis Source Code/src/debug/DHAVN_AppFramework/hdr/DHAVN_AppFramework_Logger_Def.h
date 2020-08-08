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
 * logger.Log( "Test Log Low", FacebookLogger::PRIORITY_LOW );      
 * logger.Log( "Test Log Medium", FacebookLogger::PRIORITY_MEDIUM );
 * logger.Log( "Test Log Hi", FacebookLogger::PRIORITY_HI );        
 * logger.Log( "Test Log Crt", FacebookLogger::PRIORITY_CRITICAL ); 
 *
 */
 
#if !defined(EA_9121890D_624A_47a7_8412_51BE5B74495F__INCLUDED_)
#define EA_9121890D_624A_47a7_8412_51BE5B74495F__INCLUDED_

#include "DHAVN_AppFramework_Def.h"
#include "DHAVN_LogUtil.h"
#include "DHAVN_LogDef.h"
#include <QString>
#include <QFile>
#include <QMutex>

void RedirectLogs( QtMsgType aType, const char *aMsg );


//Private class declaration
class LoggerPrivate;

class APP_FRAMEWORK_EXPORT Logger : public QObject
{
	Q_OBJECT
public:

   enum LOG_TYPE_T
   {
      LOG_NONE,
      LOG_CONSOLE,
      LOG_FILE
   };

   Q_ENUMS(LOG_PRIORITY)
   enum LOG_PRIORITY
   {
      PRIORITY_LOW = PRI_LOW,
      PRIORITY_MEDIUM = PRI_MEDIUM,
      PRIORITY_HI = PRI_HI,
      PRIORITY_CRITICAL = PRI_CRITICAL,
      PRIORITY_ASSERT = PRI_ASSERT,
      PRIORITY_TRACE = PRI_TRACE,
      PRIORITY_INFO = PRI_INFO,
      PRIORITY_SIGNAL = PRI_SIGNAL,
      PRIORITY_SLOT = PRI_SLOT,
      PRIORITY_TRANSITION = PRI_TRANSITION
   };

   static Logger * Instance(const QString& inModuleName);
 
   static Logger * Instance();	

   static void Destroy();

   Q_INVOKABLE void Log( const QString &aMessage, LOG_PRIORITY aPriority = PRIORITY_LOW );

   void SetLogType( LOG_TYPE_T aLogType, QString aModuleName = QString() );

   virtual ~Logger();

   bool IsLoggerEnabled() { return m_IsLoggerEnabled; }

   static void HandleSystemLogs( QtMsgType aType, const char *aMsg );



private:

   Logger ();

   Logger(const QString inAppId);

   Logger( const Logger & );
   

   void CheckEnableLoggerSetting();
   
   private:
    // future enhancement
   virtual void Reserved_1();
   virtual void Reserved_2();

private:

   bool m_IsLoggerEnabled;

   static Logger * m_pLogger;

   QMutex   m_Mutex;
   static QMutex m_instanceMutex;
   
   USE_LOG_UTIL;
   
private:

  //Private class object for AppEngineBase
    LoggerPrivate * d_ptr;
    Q_DECLARE_PRIVATE(Logger)   
      
};

static const QString m_PathToLogs = "/log_data/log/";

#endif // !defined(EA_9121890D_624A_47a7_8412_51BE5B74495F__INCLUDED_)
