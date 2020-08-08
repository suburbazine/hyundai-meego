#include "rebootserialport.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <termios.h>
#include <errno.h>
#include <unistd.h>
#include <sys/select.h>
#include "meegoeuid.h"
#include "common.h"
//#include "msg_client.h"

#define MODEMDEVICE	"/dev/ttyAM4"
#define REQ_RESET		0xCF

rebootSerialPort::rebootSerialPort(QObject * obj)
: QThread(obj)
{
            tryCount = 3;
            struct termios oldtio, newtio;
            fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY);
            if ( fd < 0 ){
                   // send_module_status(MID_MICOM, 100, MODULE_STATUS_FAIL, "Micom SerialPort open fail");
                ULOG->log("rebootSerialPort Cannot OPEN micom ttyAM4 : %s", strerror(errno));
                  //  exit(EXIT_FAILURE);
            }

            tcgetattr(fd, &oldtio);
            bzero(&newtio, sizeof(newtio));

            newtio.c_cflag = B115200  | CS8 | CLOCAL | CREAD;
            newtio.c_iflag = 0; // ICRNL
            newtio.c_oflag = 0;
            //newtio.c_lflag = ICANON;

            newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */
            newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
            newtio.c_cc[VERASE]   = 0;     /* del */
            newtio.c_cc[VKILL]    = 0;     /* @ */
            newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */
            newtio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
            newtio.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
            newtio.c_cc[VSWTC]    = 0;     /* '\0' */
            newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */
            newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
            newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
            newtio.c_cc[VEOL]     = 0;     /* '\0' */
            newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
            newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
            newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
            newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
            newtio.c_cc[VEOL2]    = 0;     /* '\0' */

            tcflush(fd, TCIFLUSH);
            if ( tcsetattr(fd, TCSANOW, &newtio) != 0){
                   // send_module_status(MID_MICOM, 100, MODULE_STATUS_FAIL, "Set Terminal IO Failed");
                 ULOG->log("rebootSerialPort Set Terminal IO Failed :");
                  //  exit(EXIT_FAILURE);
            }
}
rebootSerialPort::~rebootSerialPort(){

}
void rebootSerialPort::run(){
        unsigned char buf[1024];
        int readn;
        struct timeval tv, tv2;
        int result;

        tv.tv_sec = 2;
        tv.tv_usec = 0;
        fd_set readfds, allfds;
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);

        forever {
                allfds = readfds;
                tv2 = tv;
                if ( (result = select(fd + 1, &allfds, NULL, NULL, &tv2)) > 0){
                        if ( FD_ISSET(fd, &allfds)){
                                readn = read(fd, buf, 1024);

                                if ( readn < 0){
                                        if ( errno == EINTR)
                                                continue;

                                    //    send_module_status(MID_MICOM, 100, MODULE_STATUS_FAIL, "Does not receive any packet");
                                        ULOG->log("rebootSerialPort Does not receive any packet");
                                        break;
                                }

                                parse_cmd(buf, readn);
                        }
                }
                else if ( result == 0){
                      //  send_module_status(MID_MICOM, 0, MODULE_STATUS_WARNING, "read time out");
                    ULOG->log("rebootSerialPort read time out");
                     //   emit readUARTTimeout();
                    if( timeout())
                        break;

                }
                else{
                        break;
                }
        }
}

bool rebootSerialPort::timeout(){
    tryCount--;
     ULOG->log("rebootSerialPort::timeout() %d",tryCount);
   if ( tryCount < 0){
         ULOG->log("UART Reset Try Failed ");
           // send_module_status(MID_MICOM, 100, MODULE_STATUS_FAIL, "UPGRADE Micom Try Failed");
           // tryToExit();
         return true;
    }
    else{
        unsigned char data[2] = { REQ_RESET, 0x00 };
        writeCommand(data, 2);
        return false;
    }
}

void rebootSerialPort::parse_cmd(unsigned char * data, int size){
        unsigned char crc = 0;
        array.append((const char *)data, size);
        while ( array.count() >= 4 ){
                if ( (unsigned char)array.at(0) == 0xAA){
                        int data_size = array.at(1);
                        if ( array.count() >= data_size + 3){
                                // check CRC
                                token.clear();
                                token = array.left(data_size + 3);
                                for (int i = 0; i < token.count() - 1; ++i)
                                        crc += (unsigned char) token.at(i);

                                if ( crc == (unsigned char) token.at(token.count() - 1)){
                                        array.remove(0, data_size + 3);
                                        token.remove(0, 2);//SOS, LEN
                                        token.remove(token.count() - 1, 1); //CRC
                                        emit tokenArrived(token);
                                }
                        }
                        else{	// too short data size
                                return;
                        }
                }
                else{	// data broken, then clear buf
                        array.clear();
                        break;
                }
        }
}

void rebootSerialPort::writeCommand(unsigned char * data, unsigned char data_size){
        unsigned char buf[data_size + 3];
        unsigned char crc = 0;
        int i = 0;
        buf[0] = 0xAA;
        buf[1] = data_size;
        memcpy(buf + 2, data, data_size);
        for ( ; i < data_size + 2; ++i)
                crc += buf[i];
        buf[data_size + 2] = crc;

        usleep(10000);
        int result = writen(fd, buf, data_size + 3);

        if ( result != data_size + 3){
              //  send_module_status(MID_MICOM, 100, MODULE_STATUS_FAIL, "write to UART Failed");
             ULOG->log("rebootSerialPort write to UART Failed");
              //  exit(EXIT_FAILURE);
        }
        else{
             ULOG->log("rebootSerialPort Write OK : %d\n", data_size + 3);
               // fprintf(stderr, "Write OK : %d\n", data_size + 3);
        }
}

ssize_t rebootSerialPort::writen(int fd, unsigned char * data, size_t n){
        size_t	nleft;
        ssize_t	nwritten;
        unsigned char * ptr = data;

        nleft = n;
        while ( nleft > 0){
                if ( ( nwritten = write(fd, ptr ,nleft)) < 0){
                        if ( nleft == n)
                                return -1;
                        else
                                break;
                }
                else if ( nwritten == 0){
                        break;
                }
                nleft -= nwritten;
                ptr += nwritten;
        }
        return (n - nleft);
}
void rebootSerialPort::close(){
       // printf("close serial port\n");
        ULOG->log("rebootSerialPort close serial port \n");
        ::close(fd);
}
