#ifndef UTIL_H
#define UTIL_H

#include "type.h"

void    LEPrintf(char *sFormat, ...);
void    LEPrintf0(char *sFormat, ...);

UINT1   DEC2BCD2(UINT1 nDec);
UINT2   DEC2BCD4(UINT2 nDec);


UINT1   BCD2DEC1(UINT1 nBCD);
UINT1   BCD2DEC2(UINT1 nBCD);
UINT2   BCD2DEC4(UINT2 nBCD);

int     ISO639TOBCD(char a, char b);
int     BCDTOISO639(UINT1 nUpper, UINT1 nLower);

int     ISO3166TOBCD(char a, char b);
int     BCDTOISO3166(UINT1 nUpper, UINT1 nLower);


//{ added by jaehwan.lee 2014.06.27
#include "DHAVN_LogUtil.h"
#include "DHAVN_LogDef.h"
#include <QString>
#include <QDateTime>
#include <QFile>
#include <QMutex>
#include <QTextStream>
//} added by jaehwan.lee

void    LEPrintf(char *sFormat, ...);
void    LEPrintf0(char *sFormat, ...);

UINT1   DEC2BCD2(UINT1 nDec);
UINT2   DEC2BCD4(UINT2 nDec);


UINT1   BCD2DEC1(UINT1 nBCD);
UINT1   BCD2DEC2(UINT1 nBCD);
UINT2   BCD2DEC4(UINT2 nBCD);

int     ISO639TOBCD(char a, char b);
int     BCDTOISO639(UINT1 nUpper, UINT1 nLower);

int     ISO3166TOBCD(char a, char b);
int     BCDTOISO3166(UINT1 nUpper, UINT1 nLower);


//{ added by jaehwan.lee 2014.06.27
#define MC_LOG LoggerMC::Instance()->setLogLevel(LoggerMC::PRIORITY_LOW) << " [MC] " <<  __func__ << " "
#define MC_HIGH LoggerMC::Instance()->setLogLevel(LoggerMC::PRIORITY_HI) << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << " [MC] " <<  __func__ << " "
#define MC_CRITICAL LoggerMC::Instance()->setLogLevel(LoggerMC::PRIORITY_CRITICAL) << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << " [MC] " <<  __func__ << " "
#define PRINT_NOT_SUPPORTED MC_LOG << "not supported..." << LOG_ENDL;

class Q_DECL_EXPORT LoggerMC : public QObject
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

   static LoggerMC * Instance(const QString& inModuleName);

   static LoggerMC * Instance();

   static void Destroy();

   Q_INVOKABLE void Log( const QString &aMessage, LOG_PRIORITY aPriority = PRIORITY_LOW );

   virtual ~LoggerMC();

private:

   LoggerMC ();
   LoggerMC(const QString inAppId);

public:
   inline LoggerMC &operator<<(void*)
   {
       Log(m_logString, m_priority);
       m_Mutex.lock();
       m_stream.flush();
       m_logString.clear();
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerMC &operator<<(bool t)
   {
       m_Mutex.lock();
       m_stream <<  (t ? "true" : "false");
       m_Mutex.unlock();
       return *this;
   }

   template <class T>
   inline LoggerMC &operator<<(QList<T> &list)
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

   inline LoggerMC &operator<<(QVariant t)
   {
       m_Mutex.lock();
       m_stream << t.toString();
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerMC &operator<<(char t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }


   inline LoggerMC &operator<<(QBool t)
   {
       m_Mutex.lock();
       m_stream << (bool(t != 0) ? "true" : "false");
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerMC &operator<<(QChar t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerMC &operator<<(signed short t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerMC &operator<<(unsigned short t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerMC &operator<<(signed int t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerMC &operator<<(unsigned int t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerMC &operator<<(signed long t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }


   inline LoggerMC &operator<<(unsigned long t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerMC &operator<<(qint64 t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }


   inline LoggerMC &operator<<(quint64 t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerMC &operator<<(float t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerMC &operator<<(double t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerMC &operator<<(const char* t)
   {
       m_Mutex.lock();
       m_stream << QString::fromAscii(t);
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerMC &operator<<(const QString & t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }

   inline LoggerMC &operator<<(const QByteArray &t)
   {
       m_Mutex.lock();
       m_stream << t;
       m_Mutex.unlock();
       return *this;
   }
   inline LoggerMC &setLogLevel(LOG_PRIORITY pri)
   {
       m_priority = pri;
       return *this;
   }

private:
   QString m_logString;
   QTextStream m_stream;
   LOG_PRIORITY m_priority;
   static LoggerMC * m_pLogger;

   QMutex   m_Mutex;
   static QMutex m_instanceMutex;

   USE_LOG_UTIL;
};
//added by jaehwan.lee

#endif // UTIL_H
