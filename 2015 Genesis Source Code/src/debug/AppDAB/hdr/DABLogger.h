/**
 * FileName: DABLogger.h
 * Author: David.Bae
 * Time: 2011-12-03 11:52
 *
 * - Initial created by David BAE. : for easy logging~
 */
#ifndef DAB_LOGGER_H
#define DAB_LOGGER_H

#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QTime>

class DABLogger: public QObject
{
   Q_OBJECT

public:
    static DABLogger* instance()
    {
        static QMutex mutex;
        if (!m_Instance)
        {
            mutex.lock();

            if (!m_Instance)
                m_Instance = new DABLogger;

            mutex.unlock();
        }

        return m_Instance;
    }
    static void drop()
    {
        static QMutex mutex;
        mutex.lock();
        delete m_Instance;
        m_Instance = 0;
        mutex.unlock();
    }
    void initBroadcastingTime(void) { m_BroadcastingTime = QDateTime(); }
    void Log( const QString &aMessage);
    void LogCmdData(unsigned char* buf, int bufLen, bool isRead);
    void logOn(void){ logOnOff(true);}
    void logOff(void){ logOnOff(false);}
    void sendProcessCommand(QString cmd);
    QDate convertMJDToDate(quint32 mjd);
    quint32 convertDateToMJD(QDate date);
    QDateTime getBroadcastingTime(void) { return this->m_BroadcastingTime; }
    void setBroadcastingTime(QDate date, QTime time);

signals:

public slots:

private:
    DABLogger( QObject *pParent = 0, QString fileName="" , QString startLoggingString="");
    ~DABLogger();
    void PrepareLogFile(QObject *pParent, QString logFileName, QString startLoggingString);
    void logOnOff(bool onOff) {m_bLogOn = onOff;}
    QFile* m_pLogFile;
    QTextStream m_tsLog;
    bool m_bLogOn;
    static DABLogger* m_Instance;
    QDateTime m_BroadcastingTime;
};

#endif //DAB_LOGGER_H
