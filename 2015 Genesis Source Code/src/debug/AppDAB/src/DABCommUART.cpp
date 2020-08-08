#include "DABCommUART.h"
#include "DABLogger.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include "DABLogger.h"

#define BAUDRATE B115200
#define MODEMDEVICE "/dev/ttyAM1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */

DABCommUART::DABCommUART(QObject *parent) :  QObject(parent)
{
    qCritical() << Q_FUNC_INFO;
    isOpened = false;
    fd = 0;
    memset(&newtio, 0x00, sizeof(newtio));
    memset(&oldtio, 0x00, sizeof(oldtio));

#ifdef __DAB_UART_LOG__
    this->m_pCmdLogFile = new QFile(DAB_UART_LOG_FILE, parent);
    if (!m_pCmdLogFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        qCritical() << Q_FUNC_INFO << " : Log File Not found:" << DAB_UART_LOG_FILE;
        return;
    }
    m_tsOut.setDevice(m_pCmdLogFile);
    m_tsOut << endl << endl << "//========== Start Capturing ======= " << endl << endl;
    m_bLogOnOff = true; //Basically Log On TRUE
#endif //__DAB_UART_LOG__
}

DABCommUART::~DABCommUART()
{
#ifdef __DAB_UART_LOG__
    this->m_pCmdLogFile->close();
#endif
}


bool DABCommUART::Open()
{
    fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY /*| O_NONBLOCK*/ );
    if (fd < 0) {
        qCritical() << MODEMDEVICE << "Open Error !!";
        DABLogger::instance()->Log((QString(" DABCommUART::Open() : Open Error !! !!")));
        return false;
    }
    memset(&newtio, 0, sizeof(newtio));
    tcgetattr(fd, &oldtio); // save current port settings
    // set new port settings for canonical input processing
    newtio.c_cflag = BAUDRATE /*| CRTSCTS*/ | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR /*| ICRNL*/;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0/*ICANON*/;
    newtio.c_cc[VMIN]=2/*1*/;
    newtio.c_cc[VTIME]=0;
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);

    isOpened = true;
    qCritical() << Q_FUNC_INFO << "Open Success !! ";
    DABLogger::instance()->Log((QString(" DABCommUART::Open() : Open Succes !! !!")));
    return true;
}

bool DABCommUART::Close()
{
    if(fd)
    {
        tcsetattr(fd, TCSANOW, &oldtio);
        close(fd);
        fd = 0;
    }
    else
    {
        DABLogger::instance()->Log((QString(" DABCommUART::Close() : close failed !! !!")));
    }
    return true;
}

//int debugCount = 0;
int DABCommUART::Read(unsigned char* buf, int bufLen)
{
    int res = 0;

    if(!isOpened){
        return res;
    }

    if(fd)
    {
        res = read(fd, buf, bufLen);
        debugCmdLog(buf, res, true);
    }
    else
    {
        DABLogger::instance()->Log((QString(" DABCommUART::Write() : urart read failed !!")));
    }

    return res;
}

int DABCommUART::Write(unsigned char* buf, int bufLen)
{
    int res = 0;
    if(!isOpened){
        DABLogger::instance()->Log((QString(" DABCommUART::Write() : urart open failed !!")));
        return res;
    }

    if(fd)
    {
#if 1
        while( (bufLen != res) && (res < bufLen) ){
            if( (bufLen - res) >= 8){
                res += write(fd, &buf[res], 8);
                qDebug() << "Write 111111111111 = " << res << ",     bufLen = " << bufLen;
            }
            else{
                // int temp = (bufLen % 8);
                res += write(fd, &buf[res], (bufLen % 8));
                qDebug() << "Write 222222222222 END = " << res << ",     bufLen = " << bufLen;
            }
            usleep(1000); // 1ms sleep
        }
#else
        res = write(fd, buf, bufLen);
#endif
        debugCmdLog(buf, res, false);
    }
    else
    {
        DABLogger::instance()->Log((QString(" DABCommUART::Write() : urart write failed !!")));
    }
    qDebug() << "DABCommUART::Write res = " << res;
    return res;
}

void DABCommUART::debugCmdLog(unsigned char* buf, int bufLen, bool isRead)
{
#ifdef __DAB_UART_LOG__
    if(!this->m_bLogOnOff)
        return;

    if(isRead){
        if(bufLen == 20 && buf[4] == 0x04 && buf[5] == 0x06){
            //this is QOS Packet. Ignore this packet.
            return;
        }
        m_tsOut << "//>>>> Read Cmd Log" << endl << "quint8 readCmd[]={" << endl;
    }else{
        m_tsOut << "//"<<endl<<"//      <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Write Cmd Log .. Requested to module" << endl << "quint8 writeCmd[]={" << endl;
    }
    char buff[10];
    int count = 1;
    for(int i = 0; i < bufLen; i++, count++)
    {
        if(isRead && (buf[i] == 0xA5)) {
            count = 1;
            m_tsOut << endl;
        }
        sprintf(buff, " 0x%02X,",buf[i]);
        //QString str(" 0x%02X,",buf[i]);
        m_tsOut << buff;
        if((count % 20) == 0) m_tsOut << endl;
    }
    m_tsOut << endl << "}; " << endl << flush;
#else
    Q_UNUSED(buf);
    Q_UNUSED(bufLen);
    Q_UNUSED(isRead);
#endif //__DAB_UART_LOG__
}

