#ifndef	REBOOT_H_
#define	REBOOT_H_

class RebootUART;
class rebootSerialPort;
class Reboot{
public:
        Reboot(bool usingUART,bool fail); 
	~Reboot();
         RebootUART *rebootUART;
         rebootSerialPort *SPort;
private:


	void uartReset();
	void dbusReset();

	void writeUartCommand(int fd, unsigned char * data, unsigned char data_size);


};

#endif
