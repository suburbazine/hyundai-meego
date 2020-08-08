#ifndef DABCOMMSPI_H
#define DABCOMMSPI_H

#include <QObject>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <QDebug>
#ifdef __DAB_SPI_LOG__
#include <QFile>
#include <QTextStream>
#endif //__DAB_SPI_LOG__


class DABCommSPI : public QObject
{
    Q_OBJECT
public:
    explicit DABCommSPI(QObject *parent = 0);
    virtual ~DABCommSPI();
    static const quint32 MAX_SPI_PACKET_SIZE = 2048; // 2 K byte

    bool Open();
    bool Close();

    qint32 Read(quint8 *buffer, quint32 length);
    qint32 Write(quint8 *buffer, quint32 length);
    qint32 ReadWrite(quint8 *readbuf, quint8 *writebuf, quint32 length);

signals:

public slots:

private:
    qint32 m_fd;
    quint8 m_mode;
    quint8 m_bits;
    quint32 m_speed;

    quint8 m_tx[MAX_SPI_PACKET_SIZE];
    quint8 m_rx[MAX_SPI_PACKET_SIZE];

    struct spi_ioc_transfer m_tr;

#ifdef __DAB_SPI_LOG__
    // Log file for command capturing
    QFile* m_pCmdLogFile;
    QTextStream m_tsOut;
    bool m_bLogOnOff;
    void debugCmdLog(unsigned char* buf, int bufLen, int cnt);
#endif //__DAB_SPI_LOG__
};

#endif // DABCOMMSPI_H
