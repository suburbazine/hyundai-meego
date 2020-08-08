/*
    File:       RoleSwitcher.cpp
    Package:    LG iAP2 Service
    Author:     Jihwan Cho (jihwan.cho@lge.com)

    Copyright © [2014-2016] by LG Electronics Inc.

    This program or software including the accompanying associated documentation
    (“Software”) is the proprietary software of LG Electronics Inc. and or its
    licensors, and may only be used, duplicated, modified or distributed pursuant
    the terms and conditions of a separate written license agreement between you
    and LG Electronics Inc. (“Authorized License”). Except as set forth in an
    Authorized License, LG Electronics Inc. grants no license (express or implied),
    rights to use, or waiver of any kind with respect to the Software, and LG
    Electronics Inc. expressly reserves all rights in and to the Software and all
    intellectual property therein. If you have no Authorized License, then you have
    no rights to use the Software in any ways, and should immediately notify LG
    Electronics Inc. and discontinue all use of the Software.
*/

#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <sstream>
#include <fcntl.h>

#include "RoleSwitcher.h"
#include "iAp2ServiceLog.h"
#include "utils.h"
#include "SharedContext.h"
//#include "iAP2LinkRunLoop.h"

//#define _IFCONFIG_UP_

#ifdef _IFCONFIG_UP_
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif // #ifdef _IFCONFIG_UP_

#ifdef MEEGO
#include <string.h>
#include <error.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#endif //#ifdef MEEGO

#ifdef ANDROID
#include <cutils/properties.h>
#define USBCURMODEPROPERTY "sys.usb.curmode"
#define USBMODEPROPERTY "sys.usb.mode"
#define USBCONFIGPROPERTY "sys.usb.config"
#define HOSTMODE "usb_host"
#define DEVICEMODE "usb_device"

//for MCH Hub, Will be deleted
#define APPLE_VID 	0x05ac
#define APPLE_PID  	0x12a8
#define MCH_VID		0x0424
#define MCH_PID		0x2530
#define CTRL_TIMEOUT (5*1000)
#endif // #ifdef ANDROID

namespace IAP2
{
#ifdef _IFCONFIG_UP_
int set_ip(char *iface_name, char *ip_addr);
#endif // #ifdef _IFCONFIG_UP_

static void* roleSwitcherThreadFunc(void* data)
{
	RoleSwitcher* roleSwitcherThread = (RoleSwitcher*) data;
	roleSwitcherThread->roleSwitcherThread(data);
	return NULL;
}

#ifdef MM2014
/*
 * PIO18 acts as a switch to control which port to act as flex port in miniBoB (Port1 or port2).
 *
 * Direction of PIO18 should be configured in OUT mode (0x0831 |= 0x04)
 * To configure port1 as flex port PIO18_OUT register should be ‘1 ‘(0x0835 |= 0x04).
 * To configure port2 as flex port PIO18_OUT register should be ‘0’ (0x0835 &= ~ 0x04).
 *
 * After poweron reset, PIO18 is unknown state, but there is a default pullup is available on the 
 * PIO18.	So it will detect the first physical port on port 1 and next on port 2. this is checking by "byDirectionflag"
 *
 * while USB enumeration, if if PIO18 is set, then first physical port on port 1 and next on port 2.
 * if In other cases, if PIO18 is zero, (this case  will come if the iphone is removed from port 2), then it will detect port 1 as port2 and port2 as port 1.
 *
 * */

int SelectUSBUpstreamPort(struct libusb_device_handle* handle, int port_num)
{
	
	int bRetVal 	= FALSE;
	uint8_t byData = 0, byDirection =0;


	bRetVal = libusb_control_transfer(handle, 	LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_INTERFACE,
								0x04, 0x0831, 0, &byDirection, 1, CTRL_TIMEOUT);
	if ( bRetVal < 0 )	{
		LOGE("HCH Hub PIO18 Configuration READ FAILED %d",bRetVal);
		return bRetVal;
	}


	bRetVal = libusb_control_transfer(handle, 	LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_INTERFACE,
								0x04, 0x0835, 0, &byData, 1, CTRL_TIMEOUT);
	if ( bRetVal < 0 )	{
		LOGE("MCH Hub PIO18 READ FAILED %d",bRetVal);
		return bRetVal;
	}
	if( port_num == 1) {
		byData |= 0x04;
		byDirection |= 0x04;
	} else if( port_num == 2) 	{
		byData &= ~0x04;
		byDirection |= 0x04;
	}
	
	bRetVal = libusb_control_transfer(handle, LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_INTERFACE,
									0x03, 0x835,	0, &byData, 1, 	CTRL_TIMEOUT);
	if ( bRetVal< 0 ) {
		LOGE("MCH Hub PIO18 WRITE FAILED %d",bRetVal);
		return bRetVal;
	}
	bRetVal = libusb_control_transfer(handle, LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_INTERFACE,
									0x03, 0x831,	0, &byDirection, 1, 	CTRL_TIMEOUT);
	if(bRetVal< 0)
	{
		printf("XDATA WRITE FIALED %d\n",bRetVal);
	}

	return bRetVal;
}

int CheckMchHub() 
{
	libusb_device_handle *handleHCE = NULL, *handleTarget = NULL;
	libusb_device **devices;
	libusb_device *dev;
	libusb_device_descriptor desc;
	 
	libusb_context *ctx ;
	int dRetval = 0;
	ssize_t devCnt = 0, port_cnt = 0;
	int  i = 0;
	uint8_t port_list_HCE[7]; 
	ssize_t HCEdevNumber = 0, TargetdevNumber =0;
	uint8_t port_list_Target[7];  //android device or IOS device.
	char target_port_num = -1;
		

	libusb_init (&ctx);
	
	devCnt = libusb_get_device_list(ctx, &devices);
	if (devCnt < 0) {
		LOGE("Enumeration failed \n");
		return -1;
	}
	
	LOGD("Enumeration =================");
	for (i = 0; i < devCnt; i++) {
		dev = devices[i];
		
		dRetval = libusb_get_device_descriptor(dev, &desc);
		if (dRetval < 0) {
			LOGE("Cannot get the device descriptor \n");
			continue;
		}

		LOGD ("***[%04d]  VID:PID %04x:%04x", i, desc.idVendor, desc.idProduct);	
		
		if ((dRetval ==  0) && (desc.idVendor == MCH_VID) && (desc.idProduct == MCH_PID) ) {
			LOGD ("Found MCH HUB Device");
			dRetval = libusb_open(dev, &handleHCE);
			if (dRetval < 0) {
				LOGE("MCH HUB Device open failed");
				continue;
			}
			port_cnt = libusb_get_port_numbers(dev, port_list_HCE, 7);
			if (port_cnt <= 1) {
				LOGE("Retrieving port numbers  MCH HUB failed");
				libusb_close(handleHCE);
			}
			HCEdevNumber = port_cnt;
			continue;	
		}
		if ((dRetval ==  0) && (desc.idVendor == APPLE_VID) && (desc.idProduct == APPLE_PID)) {
						
			LOGD ("Found Apple Device");
			
			dRetval = libusb_open(dev, &handleTarget);
			if (dRetval < 0) {
				LOGE("Apple Device open failed");
				continue;
			}
			port_cnt = libusb_get_port_numbers(dev, port_list_Target, 7);
			if (port_cnt <= 1) {
				LOGE("Retrieving port numbers Apple Device failed");
				libusb_close(handleTarget);
			}
			TargetdevNumber = port_cnt;
			continue;	
		}

	}
	LOGD ("The connected device is: HCEdevNumber[%d]  TargetdevNumber[%d]",  HCEdevNumber, TargetdevNumber);
	//If no device is present , the wait for the device.
	if (HCEdevNumber == 0 || TargetdevNumber == 0 ) {
		LOGE("**** Error: Please check the MCHP-HUB is connected to Head Unit or");
		LOGE("                 check whether HCE is enabled on MCHP hub or");
		LOGE("                 check whether Apple is connected to MCHP hub");
		libusb_free_device_list(devices, 1);
		if (ctx)
		libusb_exit(ctx);
		return -1;
	}
	
	LOGD("HCEdevNumber (%d)  TargetdevNumber(%d)", HCEdevNumber, TargetdevNumber);
	if (HCEdevNumber == TargetdevNumber)  { 		//For make sure both are connected 4604 hub.
		for(i = 0; i < port_cnt; i++) 	{
			if( (port_list_HCE[i] != port_list_Target[i]) && i) {
				target_port_num = port_list_Target[i] ;				
				break;
			}
		}		
	}
	LOGD("Select USB Up Stream port (%d)", target_port_num);
	SelectUSBUpstreamPort(handleHCE, target_port_num);

	LOGD("Send Enable_Flex data");
	dRetval = libusb_control_transfer(handleHCE, LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_INTERFACE,
									0x08, 0xc04c, 0, NULL, 0, CTRL_TIMEOUT);

	if (dRetval< 0) {
		LOGE("Execute Enable Flex failed %d\n",dRetval);
		dRetval = (int)(0x0004);		//< API of the libusb library failed >
	}	
	libusb_free_device_list(devices, 1);
	if (ctx)
		libusb_exit(ctx);
	
	return dRetval;
}
#endif

#ifdef _IFCONFIG_UP_
		//sleep(1);
		set_ip("ncm0", NULL);
#endif // #ifdef _IFCONFIG_UP_

#ifdef MEEGO
int RoleSwitcher::netlinkFunc(int request)
{
        int ret;
        struct iovec iov;
        struct msghdr msg;

        int sk_fd;
        struct sockaddr_nl src_addr;
        struct sockaddr_nl dest_addr;
        struct nlmsghdr *send_nlh;
        //struct nlmsghdr *recv_nlh;

        unsigned int send_msg;

        sk_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_TEST);
        if(sk_fd < 0){
            LOGD("Fail to create socket for netlink\n");
            return 1;
        }

		memset(&src_addr, 0, sizeof(src_addr));
        memset(&dest_addr, 0, sizeof(dest_addr));
        
	//this iov, msg should be set by 0
		memset(&iov, 0, sizeof(iov));
        memset(&msg, 0, sizeof(msg));
        //Memory structure .. .bss .text .data

        src_addr.nl_family = AF_NETLINK;
        src_addr.nl_pid = getpid();
        //src_addr.nl_groups = 1;
        src_addr.nl_groups = 0;

        bind(sk_fd, (struct sockaddr *)&src_addr, sizeof(src_addr));

        dest_addr.nl_family = AF_NETLINK;
        dest_addr.nl_pid =0;
        //dest_addr.nl_groups = 1;
        dest_addr.nl_groups = 0;


        send_nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_NLMSG_LEN));
        if(!send_nlh){
            LOGD("Fail to allocate memory for send_nlh");
            goto err;
        }

        memset(send_nlh, 0, NLMSG_SPACE(MAX_NLMSG_LEN));
        send_nlh->nlmsg_len = NLMSG_SPACE(MAX_NLMSG_LEN);
        send_nlh->nlmsg_pid = getpid();
        send_nlh->nlmsg_flags = 0;

        /* send messge to kernel */
        switch(request){
            case DEVICE_MODE:
                    send_msg = ROLE_SWAP_REQUEST;
                    LOGD(" ######### netlink ROLE_SWAP_REQUEST");
                    LOGD(" ######### ROLE_SWAP (%d)", send_msg);
                    LOGI("sk_fd = (%d), send_msg = (%d)", sk_fd, send_msg);
                    break;

            case CLEAN_REQUEST:
                    send_msg = CLEAN_UP_REQUEST;
                    LOGD(" ######### netlink CLEAN_UP_REQUEST");
                    LOGD(" ######### CLEAN_UP (%d)", send_msg);
                    LOGI("sk_fd = (%d), send_msg = (%d)", sk_fd, send_msg);
                    break;


            default :
                LOGD("%s: Warning! undefined message",__func__);
		free(send_nlh); 
		send_nlh = NULL;
		if(sk_fd >= 0)
			ret = close(sk_fd);
		return 1;
        }

        //LOGD(" send_msg = (%d)", send_msg);

        strcpy((char *)NLMSG_DATA(send_nlh), (char *)&send_msg);
        memcpy(NLMSG_DATA(send_nlh), (void *)&send_msg, sizeof(send_msg));

        iov.iov_base = (void *)send_nlh;
        iov.iov_len = send_nlh->nlmsg_len;

        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        msg.msg_name = (void *)&dest_addr;
        msg.msg_namelen = sizeof(dest_addr);

        //LOGD(" nlmsg_len = (%d)\n", send_nlh->nlmsg_len);
        LOGD("sk_fd = (%d), send_msg = (%d)", sk_fd, send_msg);
        ret = sendmsg(sk_fd, &msg, 0);
        if(ret < 0){
            LOGD("error[%d] Fail to send data\n", ret);
            goto err;
        }
        LOGD("sk_fd = (%d), send_msg = (%d)", sk_fd, send_msg);
        LOGD("Send messge to kernel sucessfully\n");

	free(send_nlh); // jungyeon.kim
	send_nlh = NULL;
	iov.iov_base = NULL;

    err:
	if(sk_fd >= 0)
		ret = close(sk_fd);

        return 0;
    }
#endif //ifdef MEEGO


RoleSwitcher::RoleSwitcher(int vendorId, int productId, bool bCarPlaySupport) : mTid(0), mVendorId(vendorId), mProductId(productId), mbCarPlaySupport(bCarPlaySupport)
{
	LOGD("RoleSwitcher() mVendorId[0x%04x] mProductId[0x%04x] mbCarPlaySupport[%d]", mVendorId, mProductId, mbCarPlaySupport);

	mphDev = NULL;
	mpUsbCtx = NULL;

	pthread_create(&mThread, NULL, roleSwitcherThreadFunc, (void*)this);
}

RoleSwitcher::~RoleSwitcher()
{
	int retVal = 0;
	void* pThreadResult = NULL;

	LOGV("~RoleSwitcher()");


	retVal = pthread_join(mThread, &pThreadResult);
	if (retVal != 0)
		LOGE("	Thread join failed	error[%d][%s]", errno, strerror(errno));
	else
		LOGD("	roleSwitcherThread is Terminated!!!");


	if (mphDev)
		libusb_close(mphDev);

	if (mpUsbCtx)
		libusb_exit(mpUsbCtx);
}

void* RoleSwitcher::roleSwitcherThread(void* data)
{
	int rc = 0;
	int ret = 1;
    SharedContext* pShCtx = SharedContext::getInstance();

	(void) data;
	mTid= getTid();
	LOGV("roleSwitcherThread() mTid[%d]", mTid);

	rc = libusb_init(&mpUsbCtx);
	if (rc < 0) {
        LOGE("Error initializing libusb: rc[%d][%s]", rc, libusb_error_name(rc));
		pthread_mutex_lock(&pShCtx->mMutexRs);
		pShCtx->retVal = ret;
		pthread_cond_signal(&pShCtx->mCondRs);
		pthread_mutex_unlock(&pShCtx->mMutexRs);
        return NULL;
	}

	LOGV("	mVendorId[0x%04x] mProductId[0x%04x]", mVendorId, mProductId);

	#ifdef MEEGO
	mphDev = open_device_with_bus_num(mpUsbCtx);

	#else
	//MIBGP, PSA, GEELY, ANDROID	
	mphDev = libusb_open_device_with_vid_pid(mpUsbCtx, mVendorId, mProductId);
	#endif
	
	if (mphDev == NULL) {
		LOGE("	ERROR : not finding USB device");
		pthread_mutex_lock(&pShCtx->mMutexRs);
		pShCtx->retVal = ret;
		pthread_cond_signal(&pShCtx->mCondRs);
		pthread_mutex_unlock(&pShCtx->mMutexRs);
		return NULL;
	}
	else {
		LOGD("	finding USB device. mphDev[%p]", mphDev);
		ret = startRoleSwitch(mbCarPlaySupport);
	}
	
	if(ret != 0){
	//when fail to send role switch packet to iphone, UsbDeviceMonitor thread should get not only success signal but also fail signal.
		pthread_mutex_lock(&pShCtx->mMutexRs);
		LOGD("	role switch failed");
		pShCtx->retVal = ret;
		pthread_cond_signal(&pShCtx->mCondRs);
		pthread_mutex_unlock(&pShCtx->mMutexRs);
	
	}
	else {
#ifdef MEEGO
		LOGD("######netlinkFunc -> Change to Device Mode");
          	netlinkFunc(DEVICE_MODE);

#elif defined MM2014
		CheckMchHub();
		
		LOGD("usb_conf : iap2, ncm");
		property_set(USBCONFIGPROPERTY, "iap2,ncm");

		FILE* fp = fopen("/proc/driver/usbroleswitch/0/mode", "w");
		fputs("1", fp);
		fclose(fp);

		LOGD(" ***** called usbroleswitch/0/mode is 1");
		char usb_mode[PROPERTY_VALUE_MAX];    
		char usb_conf[PROPERTY_VALUE_MAX];    
		property_get(USBCURMODEPROPERTY, usb_mode, "");    
								
		LOGD("usb_conf : %s", usb_conf);
			
		//usleep(500*1000);
		system("ifconfig ncm0 up");
		//LOGD(" ***** called ifconfig ncm0");
		system("echo 0 > /proc/sys/net/ipv6/conf/ncm0/accept_dad");
#elif defined KOECN
		LOGD("Test for KOECN");
		system("setusbmode device");

#elif defined MIBGP
		LOGD("change to Device mode");
		system("echo 1 > /proc/driver/usbroleswitch/0/mode");
		system("ifconfig ncm0 up");
#elif defined GEELY
		LOGD("Role Switch Geely Board to Device Mode");
		//system("/home/root/usb_role_to_device.sh");

		char buffer[4] = {0};
		FILE* fp = fopen("/dev/mfd-iap2", "w");
		if (!fp)
			LOGE("mfd-iap2 doesn't exist");
		buffer[0] = IMX_USB1_MODE_IS_DEVICE;
		fwrite(buffer, 1, 1, fp);
		fclose(fp);

		usleep(100*1000);
		system("ifconfig ncm0 up");
#elif defined PSA
		LOGD("Role Switch PSA Board to Device Mode");
		system("ifconfig ncm0 up");

		usleep(1000*1000);
		LOGD("Ansoo FP write 100ms delay!!! ");
		FILE* fp = fopen("/sys/bus/platform/devices/ci_hdrc.0/inputs/a_bus_req", "w");
		
		fputs("0", fp);
		fclose(fp);
#endif //#ifdef MEEGO
		
		pthread_mutex_lock(&pShCtx->mMutexRs);
		pShCtx->mbRsSuccess = true;
		pShCtx->retVal = ret;
		LOGD("	respond for role switching");
		pthread_cond_signal(&pShCtx->mCondRs);
		pthread_mutex_unlock(&pShCtx->mMutexRs);
	}
	
	LOGV("roleSwitcherThread() Terminated mTid[%d]", mTid);

	return NULL;

}

int RoleSwitcher::startRoleSwitch(bool bCarPlaySupport) {
	int rc;

	LOGD("startRoleSwitch() bCarPlaySupport[%d]", bCarPlaySupport);

	if (mphDev == NULL) {
		LOGE("Error : mphDev is NULL");
		return -1;
	}

	rc = sendRoleSwitchPacket(mphDev, bCarPlaySupport);
	//LOGD(" after call sendRoleSwitchPacket()");
	if (rc < 0) {
		LOGE("Error : rc[%d][%s]", rc, libusb_error_name(rc));
		return rc;
	}

	return rc;
}

int RoleSwitcher::sendRoleSwitchPacket(struct libusb_device_handle* phDev, bool bCarPlaySupport)
{
	int rc;

	// for test
	//bCarPlaySupport = false;

	if (bCarPlaySupport == true) {
		rc = libusb_control_transfer(phDev, 0x40, 0x51, 0x01, 0x00, NULL, 0, 0);
		LOGD("############Transfer Role Switch Request to iPhone at this time");
	}
	else {
		rc = libusb_control_transfer(phDev, 0x40, 0x51, 0x00, 0x00, NULL, 0, 0);
	}

	if (rc < 0) {
		//LOGE("	ERROR : rc[%d]", rc);
		return rc;
	}

	LOGD("	== send packet size[%d]", rc);

	

	return 0;
}

#ifdef MEEGO
libusb_device_handle* RoleSwitcher::open_device_with_bus_num(libusb_context* mCtx)
{
	struct libusb_device **devs;
	struct libusb_device *found = NULL;
	struct libusb_device *dev;
	struct libusb_device_handle *handle = NULL;
	size_t i = 0;
	int r;
	int bus_num;

	if (libusb_get_device_list(mCtx, &devs) < 0)
		return NULL;

	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0)
			goto out;
		if (desc.idVendor == mVendorId && desc.idProduct == mProductId) {
			bus_num = libusb_get_bus_number(dev);
			if (bus_num == FRONT_SEAT_USB_NUM) {
				found = dev;
				break;
			}
		}
	}

	if (found) {
		r = libusb_open(found, &handle);
		if (r < 0)
			handle = NULL;
	}
	
out:
	libusb_free_device_list(devs, 1);
	return handle;
}
#endif

#ifdef _IFCONFIG_UP_
int set_ip(char *iface_name, char *ip_addr)
{
	LOGD("	set_ip() iface_name[%s]", iface_name);

	if(!iface_name)
		return -1;	
 
	int sockfd;
	struct ifreq ifr;
	struct sockaddr_in sin;
 
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd == -1){
		LOGE("	ERROR : Could not get socket");
		return -1;
	}
 
	/* get interface name */
	strncpy(ifr.ifr_name, iface_name, IFNAMSIZ);
 
	/* Read interface flags */
	if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0) {
		LOGE("	ERROR : ifdown: shutdown ");
		perror(ifr.ifr_name);
		return -1;
	}
 
	/*
	* Expected in <net/if.h> according to
	* "UNIX Network Programming".
	*/
	#ifdef ifr_flags
	# define IRFFLAGS       ifr_flags
	#else   /* Present on kFreeBSD */
	# define IRFFLAGS       ifr_flagshigh
	#endif
 
	// If interface is down, bring it up
	if (!(ifr.IRFFLAGS & IFF_UP)) {
		LOGD("	Device is currently down..setting up.-- %u", ifr.IRFFLAGS);
		ifr.IRFFLAGS |= IFF_UP;
		if (ioctl(sockfd, SIOCSIFFLAGS, &ifr) < 0) {
			LOGE("	ERROR : ifup: failed ");
			perror(ifr.ifr_name);
			return -1;
		}
	}

 	#if 0
	sin.sin_family = AF_INET;
 
	// Convert IP from numbers and dots to binary notation
    inet_aton(ip_addr, (in_addr*)&sin.sin_addr.s_addr);
	memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));	
 
	// Set interface address
	if (ioctl(sockfd, SIOCSIFADDR, &ifr) < 0) {
		LOGE("	ERROR : Cannot set IP address. ");
		perror(ifr.ifr_name);
		return -1;
	}
	#endif
	#undef IRFFLAGS		
 
	return 0;
}
#endif // #ifdef _IFCONFIG_UP_

}// namespace IAP2
