#ifndef REBOOTSERIALPORT_H
#define REBOOTSERIALPORT_H

#include <QThread>
class rebootSerialPort: public QThread{
Q_OBJECT
public:
    rebootSerialPort(QObject * obj = 0);
    ~rebootSerialPort();
    void run();
    void close();

    private:
            int	fd;
            QByteArray array;
            QByteArray token;
            void	parse_cmd(unsigned char *, int);
            ssize_t	writen(int fd, unsigned char *ptr, size_t n);
            int			tryCount;

    public:
            void writeCommand(unsigned char* data, unsigned char data_size);
            bool timeout();
    signals:
            void tokenArrived(const QByteArray &);
            void readUARTTimeout();
};

#endif // REBOOTSERIALPORT_H
