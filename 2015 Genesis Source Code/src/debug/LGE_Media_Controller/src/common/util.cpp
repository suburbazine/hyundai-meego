#include "util.h"
#include <QObject>

#include <QDateTime>
#include <QTextStream>
#include <QtDebug>
#include <DHAVN_LogUtil.h>

#define LE_DEBUG_LEVEL  0

void LEPrintf(char *sFormat, ...)
{
    Q_UNUSED(sFormat);
}

void LEPrintf0(char *sFormat, ...)
{
    Q_UNUSED(sFormat);
}

// 00~99의 숫자를 BCD로 변환 해서 리턴한다.
UINT1 DEC2BCD2(UINT1 nDec)
{
    BCD2 tBCD2;

    tBCD2.BCD.upper = nDec / 10;
    tBCD2.BCD.lower = nDec % 10;

    return tBCD2.byte;
}

// 0000~9999
UINT2 DEC2BCD4(UINT2 nDec)
{
    BCD4 tBCD4;
    
    tBCD4.BCD.upper1 = (nDec / 1000);               // X000
    tBCD4.BCD.lower1 = (nDec % 1000) / 100;         //  X00
    tBCD4.BCD.upper2 = ((nDec % 1000) % 100) / 10;  //   X0
    tBCD4.BCD.lower2 = ((nDec % 1000) % 100) % 10;  //    X
    
    return tBCD4.word;
}


// BCD 0~9의 숫자를 DEC로 변환 해서 리턴한다(이것은 BCD랑 DEC가 동일하다.)
UINT1 BCD2DEC1(UINT1 nBCD)
{
    return nBCD;
}

// BCD 00~99의 숫자를 DEC로 변환 해서 리턴한다.
UINT1 BCD2DEC2(UINT1 nBCD)
{
    BCD2    tBCD2;

    tBCD2.byte = nBCD;
    
    return tBCD2.BCD.upper * 10 + tBCD2.BCD.lower;
}

// 4 digit BCD를 DEC로 변환 해서 리턴한다.
UINT2 BCD2DEC4(UINT2 nBCD)
{
    BCD4 tBCD4;

    tBCD4.word = nBCD;

    return tBCD4.BCD.upper1 * 1000 + tBCD4.BCD.lower1 * 100 + tBCD4.BCD.upper2 * 10 + tBCD4.BCD.lower2;
}

int ISO639TOBCD(char a, char b)
{   
    int nA, nB;

    if(a >= 0x61 && a <= 0x7A) 
        a = a - 32;

    if(b >= 0x61 && b <= 0x7A) 
        b = b - 32;

    nA = a - 0x41 + 1;
    nB = b - 0x41 + 1;
    
    return nA * 100 + nB;
}

int BCDTOISO639(UINT1 nUpper, UINT1 nLower)
{
    BCD2 tBCDU, tBCDL;

    tBCDU.byte = nUpper;
    tBCDL.byte = nLower;

    return tBCDU.BCD.upper * 1000 + tBCDU.BCD.lower * 100 + tBCDL.BCD.upper * 10 + tBCDL.BCD.lower;
}

int ISO3166TOBCD(char a, char b)
{
    BCD4 tBCD4;
    BCD2 tBCDU, tBCDL;

    if(a >= 0x61 && a <= 0x7A) 
        a = a - 32;

    if(b >= 0x61 && b <= 0x7A) 
        b = b - 32;

    tBCDU.byte = a;
    tBCDL.byte = b;

    tBCD4.BCD.upper1 = tBCDU.BCD.upper;
    tBCD4.BCD.lower1 = tBCDU.BCD.lower;
    tBCD4.BCD.upper2 = tBCDL.BCD.upper;
    tBCD4.BCD.lower2 = tBCDL.BCD.lower;
    
    return tBCD4.word;
}

int BCDTOISO3166(UINT1 nUpper, UINT1 nLower)
{
    BCD4 tBCD4;
    BCD2 tBCDU, tBCDL;

    tBCDU.byte = nUpper;
    tBCDL.byte = nLower;

    tBCD4.BCD.upper1 = tBCDU.BCD.upper;
    tBCD4.BCD.lower1 = tBCDU.BCD.lower;
    tBCD4.BCD.upper2 = tBCDL.BCD.upper;
    tBCD4.BCD.lower2 = tBCDL.BCD.lower;

    return tBCD4.word;
}



LoggerMC *LoggerMC::m_pLogger = NULL;
QMutex LoggerMC::m_instanceMutex;

LoggerMC *
LoggerMC::Instance()
{
   m_instanceMutex.lock();

   if ( !m_pLogger )
   {
       m_pLogger = new LoggerMC("AppMediaPlayer");
   }

   m_instanceMutex.unlock();
   return m_pLogger;
}

LoggerMC *
LoggerMC::Instance(const QString& inModuleName)
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
        m_pLogger = new LoggerMC(inModuleName);
    }

   m_instanceMutex.unlock();
   return m_pLogger;
}

void
LoggerMC::Destroy()
{
   m_instanceMutex.lock();

   // TODO: Print to check how many times this is called
   m_pLogger->Log(QString( "Logger Instance deleted "));
   delete m_pLogger;
   m_pLogger = NULL;
   m_instanceMutex.unlock();
}

LoggerMC::LoggerMC()
    :m_priority(PRIORITY_LOW)
{
    m_Mutex.lock();
    m_logString.clear();
    m_stream.setString(&m_logString);
    m_Mutex.unlock();
}

LoggerMC::LoggerMC(const QString inLogModule)
:m_priority(PRIORITY_LOW)
{
    m_Mutex.lock();
    m_logString.clear();
    m_stream.setString(&m_logString);
    m_Mutex.unlock();

    LOG_CRITICAL << "LOG_INIT_V2" + inLogModule;
    LOG_INIT_V2(inLogModule);
    LOG_INFO<< "Logger::Logger " << inLogModule <<LOG_ENDL;

}

LoggerMC::~LoggerMC()
{
}

void
LoggerMC::Log( const QString &aMessage, LOG_PRIORITY aPriority )
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
