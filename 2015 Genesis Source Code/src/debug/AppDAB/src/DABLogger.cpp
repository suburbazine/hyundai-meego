/**
 * FileName: DABLogger.cpp
 * Author: David.Bae
 * Time: 2011-12-03 11:54
 *
 * - Initial created by David BAE. : for easy logging~
 */

#include "DABLogger.h"
#include "DABApplication_Def.h"
#include <QTime>
#include <QDebug>
#include <QProcess>

/*static*/ DABLogger* DABLogger::m_Instance = 0;

DABLogger::DABLogger( QObject *pParent, QString fileName , QString startLoggingString) :
    QObject(pParent)
{
    QTime time = QTime::currentTime();
    if(fileName.isEmpty())
    {
        fileName = DAB_APP_ENGINE_LOG_FILE;
    }

#ifndef __UNIX_DESKTOP__
    if(startLoggingString.isEmpty())
    {
        startLoggingString = "//=========== Start Logging ======= : " + time.toString("HH_mm_ss");
    }
    logOn();
    PrepareLogFile(pParent, fileName, startLoggingString);
#endif
}

DABLogger::~DABLogger()
{
    this->m_pLogFile->close();
    delete m_pLogFile;
}

void
DABLogger::PrepareLogFile(QObject *pParent, QString logFileName, QString startLoggingString)
{
#ifdef __UNIX_DESKTOP__
    return;
#else
    this->m_pLogFile = new QFile(logFileName, pParent);
    if (!m_pLogFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        qDebug() << Q_FUNC_INFO << " : Log File Not found:" << logFileName;
        return;
    }
    this->m_tsLog.setDevice(m_pLogFile);
    m_tsLog << endl << endl << startLoggingString << endl << endl;
#endif
}

void
DABLogger::Log( const QString &aMessage)
{
    m_tsLog << QTime::currentTime().toString("hh:mm:ss.zzz") << " :: " << aMessage << endl << flush;
}

void
DABLogger::LogCmdData(unsigned char* buf, int bufLen, bool isRead)
{
    if(!m_bLogOn)
        return;

    if(isRead){
        if(bufLen == 20 && buf[4] == 0x04 && buf[5] == 0x06){
            //this is QOS Packet. Ignore this packet.
            return;
        }
        m_tsLog << "//>>>> Read Cmd Log" << endl << "quint8 readCmd[]={" << endl;
    }else{
        m_tsLog << "//"<<endl<<"//      <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Write Cmd Log .. Requested to module" << endl << "quint8 writeCmd[]={" << endl;
    }
    char buff[20];
    int count = 1;
    for(int i = 0; i < bufLen; i++, count++)
    {
        if(isRead && (buf[i] == 0xA5)) {
            count = 1;
            m_tsLog << endl;
        }

        snprintf(buff, 20, "0x%02X,", buf[i]);
//        sprintf(buff, " 0x%02X,",buf[i]);
        //QString str(" 0x%02X,",buf[i]);
        m_tsLog << buff;
        if((count % 20) == 0) m_tsLog << endl;
    }
    m_tsLog << endl << "}; " << endl << flush;
}

void
DABLogger::sendProcessCommand(QString cmd)
{
    Q_UNUSED(cmd);
#ifdef __UNIX_DESKTOP__
    QProcess syncCmd;
    QByteArray ret;
    syncCmd.start(cmd);
    if(!syncCmd.waitForFinished(-1))
    {
       qDebug() << __FUNCTION__ << " Command Fail!!";
    }
    ret = syncCmd.readAll();
    qDebug() << __FUNCTION__ << " SyncCmd.readAll() ret = " << ret.data();
#endif
}

QDate
DABLogger::convertMJDToDate(quint32 mjd)
{
    QDate date = QDate::fromJulianDay(mjd+2400001);
    return date;
}

quint32
DABLogger::convertDateToMJD(QDate date)
{
    int julianDate = date.toJulianDay();
    quint32 mjd = julianDate - 2400001;
    return mjd;
}

void
DABLogger::setBroadcastingTime(QDate date, QTime time)
{
    m_BroadcastingTime.setDate(date);
    m_BroadcastingTime.setTime(time);
}
