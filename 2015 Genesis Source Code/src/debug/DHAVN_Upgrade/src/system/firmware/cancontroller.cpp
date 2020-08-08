#include "cancontroller.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/if.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <linux/can/raw.h>

#include "common.h"

extern int LCD_major;
extern int LCD_minor;

CanController::CanController(QObject * obj) : QThread(obj) {
	m_stopLoop = false;
}

CanController::~CanController() {
	ULOG->log("Can Destructor");
}

void CanController::setStopFlag(){
	ULOG->log("Set Stop Flag");
	m_stopLoop = true;
}

void CanController::setStopFlagByExternal(){
	ULOG->log("Set Stop Flag by external");
	m_stopLoop = true;
}

void CanController::run(){
	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_filter rfilter;
	struct timeval tv, tv2;
	int	major, minor;

	tv.tv_sec = CAN_READ_TIMEOUT_SEC;
	tv.tv_usec = 0;
	fd_set readfds, allfds;
	FD_ZERO(&readfds);

	m_stopLoop = false;
	ULOG->log("Start CAN");

	int s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if ( s < 0){
		ULOG_EXCEPTION;
		ULOG->log("Cannot OPEN Can Socket");
		return;
	}

	if (fcntl(s, F_SETFD, FD_CLOEXEC) == -1){
		ULOG_EXCEPTION;
		ULOG->log("Failed to CLOEXEC on socket ");
		close(s);
		return;
	}

	rfilter.can_id = 0x090;
	rfilter.can_mask = 0x080;		

	if (setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter)) == -1){
		ULOG_EXCEPTION;
		ULOG->log("Failed to setsockopt on socket ");
		close(s);
		return;
	}

	strncpy(ifr.ifr_name, "can0", sizeof(ifr.ifr_name));
	ioctl(s, SIOCGIFINDEX, &ifr);

	addr.can_family=  AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(s, (struct sockaddr *) &addr, sizeof(addr)) == -1){
		ULOG_EXCEPTION;
		ULOG->log("Failed to bind on socket ");
		close(s);
		return;
	}

	struct can_frame frame;
	frame.can_id = 0xE5;
	frame.can_dlc = 3;
	frame.data[0] = 0x05;
	frame.data[1] = 0x00;
	frame.data[2] = 0x00;
	write(s, &frame, sizeof(struct can_frame));

	FD_SET(s, &readfds);

	while( !m_stopLoop ) {
		allfds = readfds;
		tv2 = tv;

		if ( select(s + 1, &allfds, NULL, NULL, &tv2) > 0){
			if ( FD_ISSET(s, &allfds)){
				ssize_t nbytes = read(s, &frame, sizeof(struct can_frame));
				bool foundInvalid = false;
				if ( nbytes < 0){
					if ( errno == EINTR)
						continue;

					break;
				}

				ULOG->log("CAN Message : id(%04X), dlc(%d), %02X %02X %02X %02X %02X %02X %02X %02X", frame.can_id, frame.can_dlc, frame.data[0], frame.data[1], frame.data[2], frame.data[3], frame.data[4], frame.data[5], frame.data[6], frame.data[7]);

				if ( frame.can_dlc == 8 && frame.data[0] != 0xFF && frame.data[1] != 0xFF){
					major = (frame.data[0] & 0xD0) >> 5;
					minor = ( ( frame.data[1] & 0xC0) >> 6) | ( ( frame.data[0] & 0x1F ) << 2);
					if ( frame.can_id == 0x0D2){
						QByteArray ar;
						ar.append( (char)major).append( (char)minor);
                        emit canDataArrived((int) e_CCP, ar);
						ULOG->log("CCP Major(%d), Minor(%d)", major, minor);
						continue;
					}

					if ( frame.can_id == 0x0E3){
						QByteArray ar;
						ar.append( (char)major).append( (char)minor);
						emit canDataArrived((int) e_RRC, ar);
						ULOG->log("RRC Major(%d), Minor(%d)", major, minor);
						continue;
					}

					for (int i = 0; i < 8; ++i)
						if ( frame.data[i] == 0xFF)
							foundInvalid = true;

					if ( foundInvalid )
						continue;

					major = frame.data[0];
					minor = frame.data[1];

                                        if(frame.can_id ==0x0EA)
                                        {
                                            LCD_major = major;
                                            LCD_minor = minor;
                                        }

					if ( frame.can_id == 0x0EA && frame.data[0] >= 0x1C){	// front mon
						QByteArray ar;
						ar.append( (char)major).append( (char)minor);
						emit canDataArrived((int) e_FRONT_LCD, ar);
					}
                                        else if ( frame.can_id == 0x090 && frame.data[0] >= 0x24){
						QByteArray ar;
						ar.append( (char)major).append( (char)minor);
						emit canDataArrived((int) e_RL_LCD, ar);
					}
                                        else if ( frame.can_id == 0x091 && frame.data[0] >= 0x24){
						QByteArray ar;
						ar.append( (char)major).append( (char)minor);
						emit canDataArrived((int) e_RR_LCD, ar);
					}
				}
			}
		}
		else{
			ULOG->log("CanController Receive Timeout");
		}
	}
	ULOG->log("End CAN Thread");
	close(s);
}
