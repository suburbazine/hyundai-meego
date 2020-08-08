#include "DHAVN_iPodController_Logger.h"
#include <QDateTime>
#define LOG_ENABLE 0
// added by sungha.choi 2013.04.12
iPodLogger::iPodLogger(QObject *parent, int /*nPort*/) :
    QObject(parent)
{
    m_pLogfile = NULL;
#if LOG_ENABLE
    if(nPort == 1)
        m_pLogfile = new QFile("/log_data/log/ipod_log1.log");
    else
        m_pLogfile = new QFile("/log_data/log/ipod_log2.log");

    m_pLogfile->open(QIODevice::Append);

    iPodLog.setDevice(m_pLogfile);

    iPodLog << "\n[iPod Controller logger Start!!]" << endl;
#endif
}

iPodLogger::~iPodLogger()
{
#if LOG_ENABLE
    if(m_pLogfile) {
        m_pLogfile->close();
        delete m_pLogfile;
    }
#endif
}

void iPodLogger::WriteLog(const QString /*logg*/)
{
#if LOG_ENABLE
    Q_UNUSED(logg);
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();

    iPodLog << date.currentDate().toString() << " " << time.currentTime().toString("hh:mm:ss.zzz") << " - " << logg << endl;
#endif
}
