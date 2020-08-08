#ifndef DHAVN_IPODCONTROLLER_LOGGER_H
#define DHAVN_IPODCONTROLLER_LOGGER_H

// added by sungha.choi 2013.04.12
#include <QObject>
#include <QFile>
#include <QTextStream>

class iPodLogger : public QObject
{
    Q_OBJECT
public:
    explicit iPodLogger(QObject *parent = 0, int nPort = 1);
    ~iPodLogger();

    void WriteLog(const QString logg);

signals:

public slots:

private:
    QFile *m_pLogfile;
    QTextStream iPodLog;
};


#endif // DHAVN_IPODCONTROLLER_LOGGER_H
