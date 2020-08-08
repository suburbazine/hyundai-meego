#ifndef APP_MEDIAPLAYER_LOGHELPER_H
#define APP_MEDIAPLAYER_LOGHELPER_H

//{ added by jaehwan.lee 2014.06.27
#include "DHAVN_AppFramework_Def.h"
#include "DHAVN_LogUtil.h"
#include "DHAVN_LogDef.h"
#include <QString>
#include <QDateTime>
#include <QFile>
#include <QMutex>
//} added by jaehwan.lee

/*#define MP_LOG qDebug() << "[FM]" << metaObject()->className() << "::" << __func__
#define FM_WARNING() qWarning() << "[FM_1]" << metaObject()->className() << "::" << __func__
#define MP_CRITICAL qCritical() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << " [FM_0]" << metaObject()->className() << "::" << __func__*/


//{ added by jaehwan.lee 2014.06.27
#define MP_LOG LoggerEx::Instance()->setLogLevel(LoggerEx::PRIORITY_LOW) << " " <<  __func__ << " "
#define MP_HIGH LoggerEx::Instance()->setLogLevel(LoggerEx::PRIORITY_HI) << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << " " <<  __func__ << " "
#define MP_CRITICAL LoggerEx::Instance()->setLogLevel(LoggerEx::PRIORITY_CRITICAL) << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << " " <<  __func__ << " "
#define MP_MEDIUM LoggerEx::Instance()->setLogLevel(LoggerEx::PRIORITY_MEDIUM) << " " <<  __func__ << " "

class Q_DECL_EXPORT LoggerEx : public QObject
{
    Q_OBJECT
public:

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

   static LoggerEx * Instance(const QString& inModuleName);

   static LoggerEx * Instance();

   static void Destroy();

   Q_INVOKABLE void Log( const QString &aMessage, LOG_PRIORITY aPriority = PRIORITY_LOW );

   virtual ~LoggerEx();

private:

   LoggerEx ();
   LoggerEx(const QString inAppId);

public:
   inline LoggerEx &operator<<(void*)
   {
       Log(m_logString, m_priority);
       m_Mutex.lock();
       m_stream.flush();
       m_logString.clear();
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerEx &operator<<(bool t)
   {
       m_Mutex.lock();
       m_stream <<  (t ? "true" : "false");
       m_Mutex.unlock();
       return *this;
   }

   template <class T>
   inline LoggerEx &operator<<(QList<T> &list)
   {
       m_Mutex.lock();
       m_stream << '(';
       for (Q_TYPENAME QList<T>::size_type i = 0; i < list.count(); ++i) {
           if (i)
               m_stream << ", ";
           m_stream << list.at(i);
       }
       m_stream << ')';
       m_Mutex.unlock();

       return *this;
   }

   inline LoggerEx &operator<<(QVariant t)
   {

       m_Mutex.lock();
       m_stream << t.toString();
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerEx &operator<<(char t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }


   inline LoggerEx &operator<<(QBool t)
   {
       m_Mutex.lock();
       m_stream << (bool(t != 0) ? "true" : "false");
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerEx &operator<<(QChar t)
   {

       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerEx &operator<<(signed short t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerEx &operator<<(unsigned short t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerEx &operator<<(signed int t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerEx &operator<<(unsigned int t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerEx &operator<<(signed long t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }


   inline LoggerEx &operator<<(unsigned long t)
   {

       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerEx &operator<<(qint64 t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }


   inline LoggerEx &operator<<(quint64 t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerEx &operator<<(float t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerEx &operator<<(double t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerEx &operator<<(const char* t)
   {
       m_Mutex.lock();
       m_stream << QString::fromAscii(t);
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerEx &operator<<(const QString & t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerEx &operator<<(const QByteArray &t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }
   inline LoggerEx &setLogLevel(LOG_PRIORITY pri)
   {
       m_priority = pri;
       return *this;
   }

private:
   QString m_logString;
   QTextStream m_stream;
   LOG_PRIORITY m_priority;
   static LoggerEx * m_pLogger;

   QMutex   m_Mutex;
   static QMutex m_instanceMutex;

   USE_LOG_UTIL;
};
//added by jaehwan.lee


#endif // APP_MEDIAPLAYER_LOGHELPER_H
