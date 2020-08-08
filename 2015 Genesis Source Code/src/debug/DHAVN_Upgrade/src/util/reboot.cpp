#include "reboot.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <termios.h>
#include <signal.h>
#include <errno.h>
#include <QDBusConnection>
#include <QDBusMessage>
#include "meegoeuid.h"
#include "common.h"
#include "rebootserialport.h"
#include "rebootuart.h"



Reboot::Reboot(bool usingUART,bool fail){ 
    ULOG->log("Reboot sync : %d",fail);

        if ( fail == false)
	::sync();
//        uartReset();
//        ::sleep(1);
//        dbusReset();

        if ( usingUART ){
		ULOG->log("UART Reset");
                dbusReset();
                ::sleep(2);
		uartReset();
                ::sleep(1);
		dbusReset();
	}
	else{
		ULOG->log("DBus Reset");
		dbusReset();
		::sleep(1);
		uartReset();
        }
}

Reboot::~Reboot(){

}


#define MICOM_REQ_RESET	0xCF
void Reboot::uartReset(){
//	const char * modem_dev = "/dev/ttyAM4";
//	struct termios oldtio, newtio;
//	unsigned char data[2] = { MICOM_REQ_RESET, 0x00 };
//	int fd = open(modem_dev, O_RDWR | O_NOCTTY);
//	if ( fd < 0 ){
//		ULOG->log("Cannot OPEN micom ttyAM4 : %s", strerror(errno));
//		exit(EXIT_FAILURE);
//	}
//	tcgetattr(fd, &oldtio);
//	bzero(&newtio, sizeof(newtio));

//	newtio.c_cflag = B115200 | CRTSCTS | CS8 | CLOCAL | CREAD;
//	newtio.c_iflag = 0; // ICRNL
//	newtio.c_oflag = 0;
//	//newtio.c_lflag = ICANON;

//	newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */
//	newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
//	newtio.c_cc[VERASE]   = 0;     /* del */
//	newtio.c_cc[VKILL]    = 0;     /* @ */
//	newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */
//	newtio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
//	newtio.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
//	newtio.c_cc[VSWTC]    = 0;     /* '\0' */
//	newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */
//	newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
//	newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
//	newtio.c_cc[VEOL]     = 0;     /* '\0' */
//	newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
//	newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
//	newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
//	newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
//	newtio.c_cc[VEOL2]    = 0;     /* '\0' */
	
//	tcflush(fd, TCIFLUSH);
//	tcsetattr(fd, TCSANOW, &newtio);
//	writeUartCommand(fd, data, 2);
//	::close(fd);

      //  rebootSerialPort port;
      //  port.start();
       SPort = new rebootSerialPort();
       SPort->start();
       // RebootUART rebootUART(&port);
       // rebootUART = new RebootUART(&port);
       rebootUART = new RebootUART(SPort);

        //rebootUART.resetCommand();
        rebootUART->resetCommand();

}




void Reboot::dbusReset(){
	MeegoEUID euid;
        ULOG->log("DBus Reset START");
	QDBusConnection conn = QDBusConnection::sessionBus();
	QDBusMessage msg;
	QDBusMessage endMsg;
	if ( !conn.isConnected()){
		ULOG->log("Connect to Session Bus Failed");
		return;
	}
	endMsg = QDBusMessage::createMethodCall("com.lge.car.micom",
									"/System", "com.lge.car.micom.system", "LgeDBus_MainImageUpgrade");
	uchar value = 0x00;
	endMsg << qVariantFromValue(value);

	if ( !conn.send(endMsg)){
		ULOG->log("Failed to send MainImageUpgrade End");
	}


	msg = QDBusMessage::createMethodCall("com.lge.car.micom",
			"/System", "com.lge.car.micom.system", "LgeDBus_ResetHWPeripheral");
	QByteArray array;
	array.append( (char)0);
	array.append( (char)0);
	msg << qVariantFromValue(array);
        ULOG->log("DBus Reset Send");
	if (!conn.send(msg)){
		ULOG->log("send reset message to mmus failed");
	}
}

/*
void Reboot::writeUartCommand(int fd, unsigned char * data, unsigned char data_size){
	unsigned char buf[data_size + 3];
	unsigned char crc = 0;
	int i = 0;
	buf[0] = 0xAA;
	buf[1] = data_size;
	::memcpy(buf + 2, data, data_size);	
	for ( ; i < data_size + 2; ++i)
		crc += buf[i];
	buf[data_size + 2] = crc;

	::write(fd, buf, data_size + 3);
}*/

