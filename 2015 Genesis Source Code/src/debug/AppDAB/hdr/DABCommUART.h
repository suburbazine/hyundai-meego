#ifndef DABCOMMUART_H
#define DABCOMMUART_H

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/signal.h>
#include <QMutex>
#include <QObject>

#ifdef __DAB_UART_LOG__
#include <QFile>
#include <QTextStream>
#endif //__DAB_UART_LOG__
#include "DABApplication_Def.h"
#include "DABProtocol.h"

struct termios;

class DABCommUART : public QObject
{
    Q_OBJECT
public:
    explicit DABCommUART(QObject *parent = 0);
    ~DABCommUART();

    bool Open();
    bool Close();

    int Read(unsigned char* buf, int bufLen);
    int Write(unsigned char* buf, int bufLen);

signals:
    //static void readFromUART(unsigned char* buf, int bufLen);

public slots:

private:
    struct termios oldtio, newtio;
    int fd;    
    bool isOpened;
#ifdef __DAB_UART_LOG__
    // Log file for command capturing
    QFile* m_pCmdLogFile;
    QTextStream m_tsOut;
    bool m_bLogOnOff;
    QMutex   m_Mutex;
#endif //__DAB_UART_LOG__
    void debugCmdLog(unsigned char* buf, int bufLen, bool isRead);
    void debugPrint(unsigned char* buf, int bufLen);
};

#endif // DABCOMMUART_H
