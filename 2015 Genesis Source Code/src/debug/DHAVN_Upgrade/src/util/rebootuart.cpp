#include "rebootuart.h"
#include "rebootserialport.h"
#include <QCoreApplication>
#include "common.h"
#include <QtEndian>
#include <QTimer>

#define RES_RESET		0xDF
#define REQ_RESET		0xCF


RebootUART::RebootUART(rebootSerialPort * _port)
: QObject(0),serialPort(_port)
{
    tryCount = 3;
   // serialPort = new rebootSerialPort();
    connect(serialPort, SIGNAL(tokenArrived(const QByteArray &)),
                            SLOT(dataReceived(const QByteArray &)));

    connect(serialPort, SIGNAL(readUARTTimeout()), SLOT(timeoutUART()));
}

RebootUART::~RebootUART(){

}
void RebootUART::resetCommand(){
    unsigned char data[2] = { REQ_RESET, 0x00 };
    serialPort->writeCommand(data, 2);
}


void RebootUART::timeoutUART(){
        tryCount--;
ULOG->log("RebootUART::timeout() %d",tryCount);
       if ( tryCount < 0){
             ULOG->log("UART Reset Try Failed ");
               // send_module_status(MID_MICOM, 100, MODULE_STATUS_FAIL, "UPGRADE Micom Try Failed");
               // tryToExit();
        }
        else{
            unsigned char data[2] = { REQ_RESET, 0x00 };
            serialPort->writeCommand(data, 2);

        }
}

void RebootUART::dataReceived(const QByteArray & data){
        unsigned char cmd = (unsigned char)data.at(0);

        switch(cmd)
        {

        case RES_RESET:

                ULOG->log("UART Reset res_reset ");
//                if ( isEmergency){
//                        unsigned char data[2] = { 0xCF, 0x00 };
//                        serialPort->writeCommand(data, 2);
//                }
                serialPort->close();
               // exit(EXIT_SUCCESS);
                break;
        default:
            ULOG->log("UART Reset dataReceived %d",cmd);
            break;
        }
}
