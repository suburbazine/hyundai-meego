#include "DABCommSPI.h"
#include <QDebug>
#include <fcntl.h>

#define SPI_DEVICE "/dev/spidev3.0"

DABCommSPI::DABCommSPI(QObject *parent) :
    QObject(parent)
{
    qDebug() << Q_FUNC_INFO;

    m_mode = 1; //SPO = 0, SPH = 1
    m_bits = 8;
    m_speed = 3900000;//5000000; //5 MHz

    m_tr.tx_buf = (quint64)m_tx;
    m_tr.rx_buf = (quint64)m_rx;
    m_tr.len = 0;
    m_tr.delay_usecs = 0;
    m_tr.speed_hz = m_speed;
    m_tr.bits_per_word = m_bits;
    m_tr.cs_change = 0;
    m_tr.pad = 0;
    m_fd = 0;
//    Open();

#ifdef __DAB_SPI_LOG__
    qDebug("SPI Log File Start");
    this->m_pCmdLogFile = new QFile(DAB_SPI_LOG_FILE, parent);
    if (!m_pCmdLogFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        qDebug() << Q_FUNC_INFO << " : Log File Not found:" << DAB_SPI_LOG_FILE;
        return;
    }
    m_tsOut.setDevice(m_pCmdLogFile);
    m_tsOut << endl << endl << "//========== Start Capturing ======= " << endl << endl;
    m_bLogOnOff = true; //Basically Log On TRUE
    qDebug("SPI Log File END");
#endif //__DAB_SPI_LOG__

}

DABCommSPI::~DABCommSPI()
{
    Close();
#ifdef __DAB_SPI_LOG__
    this->m_pCmdLogFile->close();
#endif
}

bool
DABCommSPI::Open()
{
    qint32 ret = 0;

    m_fd = open(SPI_DEVICE, O_RDWR);
    if (m_fd < 0)
    {
        qDebug("can't open %s", SPI_DEVICE);
        return false;
    }

//    spi m_mode
    ret = ioctl(m_fd, SPI_IOC_WR_MODE, &m_mode);
    if (ret == -1)
    {
        qDebug("can't set spi mode");
        return false;
    }
    ret = ioctl(m_fd, SPI_IOC_RD_MODE, &m_mode);
    if (ret == -1)
    {
        qDebug("can't get spi mode");
        return false;
    }

//    bits per word
    ret = ioctl(m_fd, SPI_IOC_WR_BITS_PER_WORD, &m_bits);
    if (ret == -1) {
        qDebug("can't set bits per word");
        return false;
    }
    ret = ioctl(m_fd, SPI_IOC_RD_BITS_PER_WORD, &m_bits);
    if (ret == -1) {
        qDebug("can't get bits per word");
        return false;
    }

//    max speed hz
    ret = ioctl(m_fd, SPI_IOC_WR_MAX_SPEED_HZ, &m_speed);
    if (ret == -1) {
        qDebug("can't set max speed hz");
        return false;
    }
    ret = ioctl(m_fd, SPI_IOC_RD_MAX_SPEED_HZ, &m_speed);
    if (ret == -1) {
        qDebug("can't get max speed hz");
        return false;
    }

    qDebug("DABCommSPI::spi mode: %d\n", m_mode);
    qDebug("DABCommSPI::bits per word: %d\n", m_bits);
    qDebug("DABCommSPI::max speed: %d Hz (%d KHz)\n", m_speed, m_speed/1000);

    return true;
}

bool
DABCommSPI::Close()
{
    if(m_fd)
    {
        qDebug("close spi dev\n");
        close(m_fd);
        return true;
    }
    else
    {
        qDebug("spi dev is not opened\n");
        return false;
    }
}

qint32
DABCommSPI::Read(quint8 *buffer, quint32 length)
{
#ifdef __DAB_SPI_LOG__
    static int count = 0;
#endif
    qint32 ret = 0;

    if(length <= MAX_SPI_PACKET_SIZE)
    {
        if(m_fd)
        {
            memset(m_tx, 0x00, length);
            m_tr.len = length;

            ret = ioctl(m_fd, SPI_IOC_MESSAGE(1), &m_tr);
            if(ret > 0)
                if(buffer != NULL)
                    memcpy(buffer, m_rx, length);
#ifdef __DAB_SPI_LOG__
            debugCmdLog(buffer, length, count);
            count++;
#endif
        }
        else
            qDebug("spi dev is not opened\n");
    }

    return ret;
}

qint32
DABCommSPI::Write(quint8 *buffer, quint32 length)
{
    qint32 ret = 0;
    m_tr.len = length;

    if(length <= MAX_SPI_PACKET_SIZE)
    {
        if(m_fd)
        {
            memcpy(m_tx, buffer, length);
            ret = ioctl(m_fd, SPI_IOC_MESSAGE(1), &m_tr);
        }
        else
            qDebug("spi dev is not opened\n");

    }

    return ret;
}

qint32
DABCommSPI::ReadWrite(quint8 *readbuf, quint8 *writebuf, quint32 length)
{
    qint32 ret = 0;

    m_tr.len = length;

    if(length <= MAX_SPI_PACKET_SIZE)
    {
        if(m_fd)
        {
            if(writebuf != NULL)
                memcpy(m_tx, writebuf, length);
            else
                memset(m_tx, 0x00, length);

            ret = ioctl(m_fd, SPI_IOC_MESSAGE(1), &m_tr);
            if(ret > 0)
                if(readbuf != NULL)
                    memcpy(readbuf, m_rx, length);
        }
        else
            qDebug("spi dev is not opened\n");

    }

    return ret;
}

#ifdef __DAB_SPI_LOG__
void
DABCommSPI::debugCmdLog(unsigned char* buf, int bufLen, int cnt)
{
#ifdef __DAB_SPI_LOG__
    if(!this->m_bLogOnOff)
        return;

    m_tsOut << endl << " >>>>>> SPI DAT Count : " << cnt << " START <<<<<<<<<<" << endl;
    m_tsOut << endl << " >>>>>> Buffer Length : " << bufLen << endl;

    char buff[10];
    int count = 1;
    for(int i = 0; i < bufLen; i++, count++)
    {
        sprintf(buff, " 0x%02X,",buf[i]);
        //QString str(" 0x%02X,",buf[i]);
        m_tsOut << buff;
        if((count % 20) == 0) m_tsOut << endl;
    }

   m_tsOut << endl << " >>>>>> SPI DAT Count : " << cnt << "END <<<<<<<<<<" << endl;
   m_tsOut.flush();

#endif //__DAB_UART_LOG__
}
#endif
