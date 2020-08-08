#ifndef REBOOTUART_H
#define REBOOTUART_H

#include <QObject>

class rebootSerialPort;

class RebootUART : public QObject{
Q_OBJECT

public:
    RebootUART(rebootSerialPort * _port);

    ~RebootUART();
    void	resetCommand();
private:
        rebootSerialPort * serialPort;
        int			tryCount;
private slots:
        void dataReceived(const QByteArray &);
        void timeoutUART();
};

#endif // REBOOTUART_H
