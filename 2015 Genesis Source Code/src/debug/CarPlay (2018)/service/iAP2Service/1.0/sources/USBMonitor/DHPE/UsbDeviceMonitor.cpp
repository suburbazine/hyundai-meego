/*
    File:       UsbDeviceMonitor.cpp
    Package:    LG iAP2 Service
    Author:     Jihwan Cho (jihwan.cho@lge.com)

    Copyright ¨Ï [2014-2016] by LG Electronics Inc.

    This program or software including the accompanying associated documentation
    (¡°Software¡±) is the proprietary software of LG Electronics Inc. and or its
    licensors, and may only be used, duplicated, modified or distributed pursuant
    the terms and conditions of a separate written license agreement between you
    and LG Electronics Inc. (¡°Authorized License¡±). Except as set forth in an
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

#include "UsbDeviceMonitor.h"
#include "iAp2ServiceLog.h"
#include "utils.h"
#include "iAP2LinkRunLoop.h"
#include "SharedContext.h"

#if !defined(MEEGO)
#include <sys/socket.h>   
#include <linux/netlink.h>   

#define UEVENT_BUFFER_SIZE (1024*20)
#endif 

#ifdef ANDROID
#include <unistd.h>
#include <cutils/properties.h>
#include <cutils/sockets.h>
#endif

#define APPLE_VENDOR_ID "05ac"
#define APPLE_PRODUCT_ID "12" //12nn

#define ERROR_IO		-1
#define ERROR_PIPE		-9
#define ROLESWITCH_SUCCESS	0

namespace IAP2
{

static void* monitorThreadFunc(void* data)
{
	UsbDeviceMonitor* monitorThread = (UsbDeviceMonitor*) data;
	monitorThread->monitorThread(data);
	return NULL;
}

UsbDeviceMonitor::UsbDeviceMonitor(struct UsbConnectCallBack* pCb) : mTid(0), mIsTerminated(false), mConnectState(LINKCTRL_USB_NOT_CONNECTED), mConnPath(CP_NONE), mFd(-1)
#if !defined(MEEGO)
											, mCarPlaySupport(0)
											, isCarPlayConnected(CARPLAY_NOT_CONNECTED)
#endif

											, mbCarPlaySupport(true)
#ifndef ANDROID
											, mpCarPlayRemoveReceiver(NULL)
#endif
											, mpRoleSwitcher(NULL)
											, mpTransport(NULL)
#ifndef ANDROID
											, gAttachedCheck(false)
#endif
{
	//mpUserData = pUserData;
	mpUserData = NULL;
	mpConnCbs = pCb;
	if(pCb == NULL)
	{
		LOGE("## ERROR UsbConnectCallBack value is null ###"); 
		LOGE("mpConnCbs : %p", mpConnCbs);
	}
	LOGD("UsbDeviceMonitor()\n");
	pthread_mutex_init(&mMutexMonThSync, NULL);
	pthread_cond_init(&mCondMonThSync, NULL);
#ifndef ANDROID
	mpCarPlayRemoveReceiver = new CarPlayRemoveReceiver(this);
#endif
	/* Create the udev object */
	mpUdev = udev_new();
	if (mpUdev == NULL) {
		LOGE("	ERROR : Can't create udev");
		return;
	}

#if !defined(MEEGO)
	LOGD("	mpUdev[%p]", mpUdev);

#else //#ifdef MEEGO
	/* Set up a monitor to monitor hidraw pDevListEntrys */
	mpMonitor = udev_monitor_new_from_netlink(mpUdev, "udev");

	if (mpMonitor == NULL) {
		LOGE("	ERROR : fail to create monitor");
		return;
	}

	LOGD("	mpUdev[%p] mpMonitor[%p]", mpUdev, mpMonitor);
#endif

	mProductInfo.vendorId = 0;
	mProductInfo.productId = 0;

	pthread_create(&mThread, NULL, monitorThreadFunc, (void*)this);

	pthread_mutex_lock(&mMutexMonThSync);
	pthread_cond_wait(&mCondMonThSync, &mMutexMonThSync);
	pthread_mutex_unlock(&mMutexMonThSync);
}

UsbDeviceMonitor::~UsbDeviceMonitor()
{
	LOGV("~UsbDeviceMonitor()");

#if !defined(MEEGO)
	int retVal = 0;
	if (mFd != -1) {
		retVal = close(mFd);
		if (retVal == -1)
			LOGE("	ERROR : fail to close mFd[%d] error[%d][%s]", mFd, errno, strerror(errno));
	}
#else// #Meego
	if (mpMonitor != NULL) {
		udev_monitor_unref (mpMonitor);
		mpMonitor = NULL;
	}
#endif

	if (mpUdev != NULL) {
		udev_unref(mpUdev);
		mpUdev = NULL;
	}

	if (mpTransport != NULL) {
		delete mpTransport;
		mpTransport = NULL;
	}
#ifndef ANDROID
	if (mpCarPlayRemoveReceiver) {
		delete mpCarPlayRemoveReceiver;
		mpCarPlayRemoveReceiver = NULL;
	}
#endif

	pthread_cond_destroy(&mCondMonThSync);
	pthread_mutex_destroy(&mMutexMonThSync);
}

#ifdef ANDROID 
struct udev_device* UsbDeviceMonitor::monitorReceiveDevice(char** ppAction, bool* isIpod)
{
	udev_device* pDev = NULL;
	int rcvLen;
	char buf[UEVENT_BUFFER_SIZE] = { 0 };  
	static char sActionStr[100];
	*isIpod = false;

	//LOGD("monitorReceiveDevice()");

	if (mFd <= 0) {
		LOGE("	ERROR : mFd[%d]", mFd);
		return NULL;
	}
	
	/* receive data */	
	//rcvLen = recv(mFd, &buf, sizeof(buf), 0);  
	rcvLen = recv(mFd, buf, sizeof(buf), 0); 
	if (rcvLen > 0) {  
		char* p;
		char path[512];
		char* pStrStart;
		char usb_action[7] = {0};
		char usb_subsystem[7] = {0};
		int i = 0;
		int j = 0;

		int nTotParsedBuf = 0;
		bool bFirst = true;
		
		do {
			pStrStart = buf + nTotParsedBuf;

			bFirst = false;
			p = pStrStart;
			while (*p != 0) {
				if (*p == '@') {
					bFirst = true;
					p++;
					break;
				}
				usb_action[i] = *p;
				p++;
				i++;
			}

			if (bFirst == true) {
				#define HIDRAWPATH "/hidraw"
				const char* pSubSystem;
				snprintf(path, sizeof(path), "%s%s", "/sys", p);
				pDev = udev_device_new_from_syspath(mpUdev, path);
				
				strncpy(sActionStr, pStrStart, p-pStrStart-1);
				sActionStr[p-pStrStart-1] = 0;
				
				*ppAction = sActionStr;

				pSubSystem = udev_device_get_subsystem(pDev);

				if (pDev) {
					*isIpod = false;
					if (pSubSystem != NULL) {
						if (!strcmp(pSubSystem, "hidraw")) {
							break;
						}
#ifdef MM2014
						else if (!strcmp(pSubSystem, "android_usb")) {
							break;
						}
#else //KOECN
						else if (!strcmp(pSubSystem, "platform") && !strcmp(path, "/sys/devices/platform/tcc-dwc3.0/dwc3.0")) {
							break;
						}
#endif

						else {
							*ppAction = (char*)"";
							udev_device_unref(pDev);
							pDev = NULL;
						}
					}
					else {
						//LOGD("	pSubSystem is NULL");
						*ppAction = (char*)"";
						udev_device_unref(pDev);
						pDev = NULL;
					}
				}
				else {
					const char* pFoundStr = NULL;
					udev_device_unref(pDev);
					pDev = NULL;
					pFoundStr = strstr(path, HIDRAWPATH);
					if (pFoundStr == NULL) {
						//LOGE("pFoundStr is NULL");
						return NULL;
					}
					for (j=0; j<(int)sizeof(usb_subsystem); j++)
						usb_subsystem[j] = pFoundStr[j+1];
					usb_subsystem[6] = 0;

					if (!strcmp(usb_action, "remove") || !strcmp(usb_subsystem, "hidraw"))
						*isIpod = true;
				}
			}

			nTotParsedBuf = nTotParsedBuf + (strlen(buf+nTotParsedBuf) + 1/*NULL*/);
		} while (nTotParsedBuf < rcvLen);

	}
	else {
		LOGW("	WARNING : rcvLen[%d]", rcvLen);
		return NULL;
	}

	return pDev;
}
#endif // #ifdef ANDROID 

void* UsbDeviceMonitor::monitorThread(void* data)
{
	fd_set fds;
	struct timeval tv;
	int ret;
	bool bSetupUsb = false;
	bool bEnumUsb = false;
#ifdef ANDROID 
	bool isIpod_Check = false;
#endif 

	char* pAction = NULL;
	string devNodPath;
	struct udev_device* pDev = NULL;

	(void) data;
	mTid= getTid();
	LOGV("monitorThread() mTid[%d]", mTid);

	pthread_mutex_lock(&mMutexMonThSync);
	pthread_cond_signal(&mCondMonThSync);
	pthread_mutex_unlock(&mMutexMonThSync);

#if !defined(MEEGO)	
	if (mpUdev == NULL ) {
		LOGE("	ERROR : mpUdev is NULL");
		return NULL;
	}
#else
	if (mpUdev == NULL || mpMonitor == NULL) {
		LOGE("	ERROR : mpUdev[%p] or mpMonitor[%p] is NULL", mpUdev, mpMonitor);
		return NULL;
	}
#endif

	// monitoring usb shoud be earlyier executed than enumeration Usb.
	bSetupUsb = setUpMonitoringUsb();
	if (!bSetupUsb) {
		LOGE("setUpMonitoringUsb Failed");
	}
	bEnumUsb = ToDeviceMode();
	if (!bEnumUsb) {
		LOGD("Apple Device is not connected when booting");
	}
	while (!mIsTerminated) {
		
		FD_ZERO(&fds);
		FD_SET(mFd, &fds);

		//	Note that the timeval object is set to 0, which will cause select() to not	block. */
		tv.tv_sec = 0;
		tv.tv_usec = 100*1000; // 100 msec
		
		ret = select(mFd+1, &fds, NULL, NULL, &tv);		
		
		if (ret < 0){
			LOGE("	ERROR : UsbDeviceMonitor_ret %d", ret);
		}
		
		if (ret > 0 && FD_ISSET(mFd, &fds)) {
#ifdef ANDROID 
			bool isIpod_Check = false;
			usleep(50*1000);  // It's time to sync device node.
			pDev = monitorReceiveDevice(&pAction, &isIpod_Check);
			if (pDev == NULL) {
				if (isIpod_Check == true && mCarPlaySupport == CCS_CARPLAY_NOT_SUPPORTED && isCarPlayConnected == CARPLAY_NOT_CONNECTED) {
					LOGD("hidraw && CARPLAY_NOT_SUPPORTED && CarPlay is Not Connected");
					if (mpConnCbs && mpConnCbs->connStateCb)
						mpConnCbs->connStateCb(mpConnCbs->pData, LINKCTRL_USB_NOT_CONNECTED, CP_NONE);
					else
						LOGE(" ERROR : connStateCb is not registed, mpConnCbs[%p]", mpConnCbs);
					mConnectState = LINKCTRL_USB_NOT_CONNECTED;
					mDevicePath.clear();
					resetProductInfo();
					if (mConnPath == USB_PORT1) {
						if (mpTransport != NULL) {
							delete mpTransport;
							mpTransport = NULL;
						}
					mConnPath = CP_NONE;
					}
				continue;
				}
				else {
					//LOGD("	pDev is NULL");
					continue;
				}
			}
#elif defined(MIBGP) || defined(GEELY) || defined(PSA)
			pDev = monitorReceiveDevice(&pAction);
			if (pDev == NULL) {
				//LOGD("pDev is NULL!!");
				continue;
			}
#elif defined (MEEGO)
			pDev = udev_monitor_receive_device(mpMonitor);

			// add, remove, change, online, offline.
			pAction = (char*)udev_device_get_action(pDev);
#endif // #if defined(ANDROID) || defined(MIBGP)

			char* pDevNode= (char*)udev_device_get_devnode(pDev);
			if (pDevNode)
				devNodPath = (char*)udev_device_get_devnode(pDev);
			else 
				devNodPath = "";

			LOGI("##### iAP2Service VERSION : %s #####", VERSION);
			LOGD("== Got Device == devNodPath[%p]", pDevNode);
			LOGD("	devNodPath[%s] Subsystem[%s]", devNodPath.c_str(), udev_device_get_subsystem(pDev));
			LOGD("	Action[%s] mConnectState[%d]", pAction, mConnectState);

			if (pDev) {
				if (!strcmp(pAction, "add")) {
					struct udev_device* pDevParent = udev_device_get_parent_with_subsystem_devtype(pDev, "usb", "usb_device");
					LOGD("	pDev : [%p]", pDevParent);
						if (pDevParent == NULL) {
							//LOGE("	ERROR : pDev is NULL");
							udev_device_unref(pDev);
							continue;
						}

						bool handleAdd = handleInsertUsb(pDevParent, devNodPath);
						if (!handleAdd) {
							udev_device_unref(pDev);
							continue;
						}
						else
							LOGD(" CarPlay will work");
					}
				else if (!strcmp(pAction, "remove")) {
					LOGD("	device is disconnected devNodPath[%s] mDevicePath[%s] mConnectState[%u] mConnPath[%u]!!!", devNodPath.c_str(), mDevicePath.c_str(), mConnectState, mConnPath);

					if (mConnectState == LINKCTRL_USB_CONNECTED) {
						if (!devNodPath.compare(mDevicePath)) {
							LOGD("	Apple device is disconnected !!!");

							if (mpConnCbs && mpConnCbs->connStateCb)
								mpConnCbs->connStateCb(mpConnCbs->pData, LINKCTRL_USB_NOT_CONNECTED, CP_NONE);
							else
								LOGE("	ERROR : connStateCb is not registered. mpConnCbs[%p]", mpConnCbs);
							
							mConnectState = LINKCTRL_USB_NOT_CONNECTED;
							mDevicePath.clear();
							resetProductInfo();
#ifndef ANDROID
							if (gAttachedCheck) {
								iAP2LinkRunLoopDetached((iAP2LinkRunLoop_t*)mpUserData);
								gAttachedCheck = false;
							}
#else
								iAP2LinkRunLoopDetached((iAP2LinkRunLoop_t*)mpUserData);
#endif
							if (mpTransport != NULL) {
								delete mpTransport;
								mpTransport = NULL;
							}
							else
								LOGE("	ERROR : mpTransport is already deleted");
							
							mConnPath = CP_NONE;
						}
					}
				}
				else if (!strcmp(pAction, "change")) {
					LOGD("	device is change devNodPath[%s] mDevicePath[%s]!!!", devNodPath.c_str(), mDevicePath.c_str());

#ifdef MM2014
#define REMOVE_PATH "/devices/virtual/android_usb/android0"
#elif defined(KOECN)
#define REMOVE_PATH "/devices/platform/tcc-dwc3.0/dwc3.0"
#elif defined(MIBGP)
#define REMOVE_PATH "/devices/platform/fsl-usb2-udc/gadget/tty/ttyIAP20"
#elif defined(GEELY)
#define REMOVE_PATH ""
#elif defined(PSA)
#define REMOVE_PATH ""
#endif 

#ifdef MM2014
#define REMOVE_EVENT "DISCONNECTED"
#elif defined(KOCEN)
#define REMOVE_EVENT "LINK_STATE_DISCONNECTED"
#else
#define REMOVE_EVENT "EVENT"
#endif


					if (mConnectState == LINKCTRL_USB_CONNECTED) {
	#ifdef MEEGO
						if (!devNodPath.compare(mDevicePath)) {
							const char* pEventName = udev_device_get_property_value(pDev, REMOVE_EVENT); 
							LOGD("	pEventName[%s]", pEventName);

							//if (!strcmp(pEventName, "suspend") && (eventCount == 1) ) {
							if (pEventName  &&  !strcmp(pEventName, "suspend")) { 	
	#elif defined(MM2014)
						const char* pDevPath = udev_device_get_devpath(pDev);
						LOGD("	pDevPath[%s]", pDevPath);
						
						if (!strcmp(pDevPath, REMOVE_PATH)) {
							const char* pEventName = (char*)udev_device_get_sysattr_value(pDev,"state");
							LOGD("	pEventName[%s]", pEventName);
							if (pEventName  &&  !strcmp(pEventName, "DISCONNECTED")) { // CONFIGURED
	#elif defined(KOECN)
	                    const char* pDevPath = udev_device_get_devpath(pDev);
						LOGD("	pDevPath[%s]", pDevPath);
						
						if (!strcmp(pDevPath, REMOVE_PATH)) {
	#elif defined(MIBGP) || defined(GEELY) || defined(PSA)
						//const char* pDevPath = udev_device_get_devpath(pDev);
						//LOGD("	pDevPath[%s]", pDevPath);
						//if (!strcmp(pDevPath, REMOVE_PATH)) {
						//	const char* pEventName = (char*)udev_device_get_property_value(pDev, REMOVE_EVENT);
						//	LOGD("	pEventName : [%s]", pEventName);
						if (1) {
	
	#endif // #ifndef ANDROID
								LOGD("	Apple device is disconnected !!!");

								if (mpConnCbs && mpConnCbs->connStateCb){
									mpConnCbs->connStateCb(mpConnCbs->pData, LINKCTRL_USB_NOT_CONNECTED, CP_NONE);
								}
								else
									LOGE("	ERROR : connStateCb is not registered. mpConnCbs[%p]", mpConnCbs);
								
								mConnectState = LINKCTRL_USB_NOT_CONNECTED;
								mDevicePath.clear();
								resetProductInfo();
#ifndef ANDROID
								if (gAttachedCheck) {
									iAP2LinkRunLoopDetached((iAP2LinkRunLoop_t*)mpUserData);
									gAttachedCheck = false;
								}
#else
								iAP2LinkRunLoopDetached((iAP2LinkRunLoop_t*)mpUserData);
#endif
								if (mpTransport != NULL) {
									// LOGD(" ###### mpTransport has deleted");
									delete mpTransport;
									mpTransport = NULL;
								}
								else
									LOGE("	ERROR : mpTransport already deleted]");

								mConnPath = CP_NONE;
								//mpConnCbs->DeleteiAP2LinkRunLoopCb(mpConnCbs->pData);
	#ifdef MEEGO
								mpRoleSwitcher->netlinkFunc(CLEAN_REQUEST);
								LOGD(" ###### netlink CLEAN_UP message sent");
	#elif defined(MM2014)	
								system("echo 0 > /proc/driver/usbroleswitch/0/mode");
								isCarPlayConnected = CARPLAY_NOT_CONNECTED;
								LOGD(" ***** called usbroleswitch/0/mode is 0");
								sleep(2);
								system("restore_port");
								LOGD("***** called restore_port");
								usleep(30*1000);
	#elif defined(KOECN)
								LOGD("Call setusbmode host");
								system("setusbmode host");
	#elif defined(MIBGP)	
								system("echo 0 > /proc/driver/usbroleswitch/0/mode");
								isCarPlayConnected = CARPLAY_NOT_CONNECTED;
								LOGD(" ***** called usbroleswitch/0/mode is 0");
								usleep(30*1000);
	#elif defined(GEELY)
								LOGD("disconnect for GEELY");
	#elif defined(PSA)
								LOGD("Role Switch PSA Board to Host Mode");
								LOGD("If unplug the phone, the device is automatically changed to host mode");
	#endif //MEEGO
	
	#if defined(MEEGO) || defined(ANDROID)
							}
	#endif
						}
	#ifndef KOECN
					}
	#endif
					else {
						#ifndef ANDROID
						LOGW("	WARNING : not expected case");
						#else
						LOGD("Disconnect, but not for iap2");
						#endif
					}
				}
				else {
					LOGD("	WARNING : unused pAction[%s]", pAction);
				}

				udev_device_unref(pDev);
			}
			else {
				LOGE("	ERROR : No Device from receive_device(). An error occured. pDev is NULL");
			}					
		}
	}	

	return NULL;
}

bool UsbDeviceMonitor::ToDeviceMode()
{
	int retVal = 0;
	
	struct udev_enumerate* pEnum;
	struct udev_list_entry* pDevListEntrys;
	struct udev_list_entry* pDevListEntry;
	struct udev_device* pDev;

	LOGD("ToDeviceMode()");

	mConnectState = LINKCTRL_USB_NOT_CONNECTED;
	mDevicePath.clear();
	resetProductInfo();

#if !defined(MEEGO)
	if (mpUdev == NULL) {
		LOGE("	ERROR : mpUdev[%p]NULL");
		return false;
	}
#else
	if (mpUdev == NULL || mpMonitor == NULL) {
		LOGE("	ERROR : mpUdev[%p] or mpMonitor[%p] is NULL", mpUdev, mpMonitor);
		return false;
	}
#endif

	/* Create a list of the pDevListEntrys in the 'hidraw' subsystem. */
	pEnum = udev_enumerate_new(mpUdev);
	if (pEnum == NULL ) {
		LOGE("	ERROR : pEnum is NULL");
		return false;
	}	
	
	retVal = udev_enumerate_add_match_subsystem(pEnum, "hidraw");
	if (retVal != 0) {
		LOGE("	ERROR : udev_enumerate_add_match_subsystem() retVal[%d] errno[%d][%s]", retVal, errno, strerror(errno));
		return false;
	}
		
	retVal = udev_enumerate_scan_devices(pEnum);
	if (retVal != 0) {
		LOGE("	ERROR : udev_enumerate_scan_devices() retVal[%d] errno[%d][%s]", retVal, errno, strerror(errno));
		return false;
	}
	
	pDevListEntrys = udev_enumerate_get_list_entry(pEnum);
	if (pDevListEntrys == NULL ) {
		LOGD("	any usb device is not connected");
		return false;
	}

	udev_list_entry_foreach(pDevListEntry, pDevListEntrys) {
		const char* pPath;
		string devNodPath;

		pPath = udev_list_entry_get_name(pDevListEntry);
		LOGD("	pPath[%s]", pPath);

		pDev = udev_device_new_from_syspath(mpUdev, pPath);
		if (pDev == NULL ) {
			//LOGE("	ERROR : pDev is NULL");
			return false;
		}

		devNodPath = udev_device_get_devnode(pDev);
		LOGD("	devNodPath[%s]", devNodPath.c_str());

		struct udev_device* pDevParent = udev_device_get_parent_with_subsystem_devtype( pDev, "usb", "usb_device");
		if (!pDevParent) {
			LOGE("	Unable to find parent usb device.");
			udev_device_unref(pDev);
			return false;
		}
		else
			LOGD("	Parent dev node Path[%s]", udev_device_get_devnode(pDevParent));

		if( isAppleDevice(pDevParent) == true ) {
			bool handleAdd = handleInsertUsb(pDevParent, devNodPath);
			if (!handleAdd) {
				LOGD(" ipod will work");
			}
			else
				LOGD(" CarPlay will work");
		}
		else {
			LOGE("Not an Apple Device");
			udev_device_unref(pDev);
			return false;
		}
	}

	udev_device_unref(pDev);

	/* Free the enumerator object */
	udev_enumerate_unref(pEnum);

	return true;
}

bool UsbDeviceMonitor::isAppleDevice(struct udev_device* pDev)
{
	char* pVendorStr;
	char* pProductStr;
	
	char product_check[3] = {0};

	int i;

	LOGD("isAppleDevice()");

	if (pDev == NULL) {
		LOGE("	ERROR : isAppleDevice() pDev is NULL");
		return false;
	}

	/* From here, we can call get_sysattr_value() for each file  in the device's /sys entry. The strings passed into these
	   functions (idProduct, idVendor, serial, etc.) correspond directly to the files in the /sys directory which
	   represents the USB device. Note that USB strings are  Unicode, UCS2 encoded, but the strings returned from
	   udev_device_get_sysattr_value() are UTF-8 encoded. */

	pVendorStr = (char*)udev_device_get_sysattr_value(pDev,"idVendor");
	pProductStr = (char*)udev_device_get_sysattr_value(pDev,"idProduct");
	if (pVendorStr == NULL) {
		LOGE("VID is NULL");
		return false;
	}
	if (pProductStr == NULL) {
		LOGE("PID is NULL");
		return false;
	}

	for( i=0; i<2; i++ ) {
		product_check[i] = pProductStr[i];
	}
	product_check[2] = (char)NULL;

	// TODO : check product id 0x12nn p488
	if( !strcmp(pVendorStr, APPLE_VENDOR_ID )) {
		if( !strcmp(product_check, APPLE_PRODUCT_ID )) {
			LOGD(" VID[%s], PID[%s], Apple Device!", pVendorStr, pProductStr);
			return true;
		}
	}

	//LOGE(" VID[%s], PID[%s], Apple Device!", pVendorStr, pProductStr);
	//LOGE("Not an Apple Device!");
	return false;
}

ConnectionPath UsbDeviceMonitor::isNormalPath(struct udev_device* pDev)
{
#if defined(MEEGO) || defined(MIBGP)
	#define SEARCH_STR		"/usb1"
	#define NORMAL_PATH_STR		"/usb1/1-1"
#elif defined(MM2014)
	#define SEARCH_STR 		"/usb2"
	#define NORMAL_PATH_STR1	"/usb2/2-1/2-1.1"
	#define NORMAL_PATH_STR2	"/usb2/2-1/2-1.2"
#endif

	const char* pDevPath = udev_device_get_devpath(pDev);
	const char* pFoundStr = NULL;
	LOGD("	pDevPath[%s]", pDevPath);

#ifndef ANDROID	
	pFoundStr = strstr(pDevPath, SEARCH_STR);
	if (pFoundStr == NULL) {
		return CP_NONE;
	}
	else {
		return USB_PORT1;
	}

#elif defined(MM2014)
	pFoundStr = strstr(pDevPath, SEARCH_STR);
	if (pFoundStr == NULL) {
		return CP_NONE;
	}
	else {
		if (!strcmp(pFoundStr, NORMAL_PATH_STR1)) {
			LOGD("PATH#1");
			return USB_PORT1;
		}
		else if (!strcmp(pFoundStr, NORMAL_PATH_STR2)) {
			LOGD("PATH#2");
			return USB_PORT2;
		}
		else {
			LOGE("Undefined Port");
			return CP_NONE;
		}
	}
#else //KOECN
	return USB_PORT1;

#endif // #if defined(MEEGO) || defined(MIBGP)
}


void UsbDeviceMonitor::setProductInfo(struct udev_device* pDev)
{	
	LOGD("setProductInfo()");

	if (pDev == NULL) {
		LOGE("	ERROR : setProductInfo() pDev is NULL");
		return;
	}

	string str;

	str = udev_device_get_sysattr_value(pDev, "idVendor");
	std::stringstream convertVId(str);
	convertVId >> std::hex >> mProductInfo.vendorId;

	str = udev_device_get_sysattr_value(pDev, "idProduct");
	std::stringstream convertPId(str);
	convertPId >> std::hex >> mProductInfo.productId;

	str = udev_device_get_sysattr_value(pDev,"manufacturer");
	mProductInfo.manufacturer.assign(str);

	str = udev_device_get_sysattr_value(pDev,"product");
	mProductInfo.product.assign(str);

	str = udev_device_get_sysattr_value(pDev,"serial");
	mProductInfo.serial.assign(str);

	LOGD("	vendorId[0x%x] productId[0x%x]", mProductInfo.vendorId, mProductInfo.productId);
	LOGD("	manufacturer[%s] product[%s]",	mProductInfo.manufacturer.c_str(),	mProductInfo.product.c_str());
	LOGD("	serial[%s]", mProductInfo.serial.c_str());
}

void UsbDeviceMonitor::resetProductInfo()
{	
	mProductInfo.vendorId = 0;
	mProductInfo.productId = 0;
	mProductInfo.manufacturer.clear();
	mProductInfo.product.clear();
	mProductInfo.serial.clear();
}

bool UsbDeviceMonitor::setUpMonitoringUsb()
{
	LOGD("setUpMonitoringUsb()");
	
#if !defined(MEEGO)
	mFd = getFdFromNetlink();
#else
	mFd = getFdFromUdev();
#endif // #if defined(ANDROID) || defined(MIBGP)
	if (mFd < 0) {
		LOGE("mFd set fail");
		return false;
	}
	LOGD("	monitor socket mFd[%d]", mFd );
	return true;
}
#if defined(MEEGO)
int UsbDeviceMonitor::getFdFromUdev()
{
	int retVal = 0;

	if (mpMonitor == NULL) {
		LOGE("	ERROR : mpMonitor is NULL");
		return -1;
	}

	retVal = udev_monitor_filter_add_match_subsystem_devtype(mpMonitor, "hidraw", NULL);
	//retVal = udev_monitor_filter_add_match_subsystem_devtype(mpMonitor, "tty", NULL);
	if (retVal != 0) {
		LOGE("	ERROR : udev_monitor_enable_receiving() retVal[%d] errno[%d][%s]", retVal, errno, strerror(errno));
		return -1;
	}

	retVal = udev_monitor_filter_add_match_subsystem_devtype(mpMonitor, "tty", NULL);
	if (retVal != 0) {
		LOGE("	ERROR : udev_monitor_enable_receiving() retVal[%d] errno[%d][%s]", retVal, errno, strerror(errno));
		return -1;
	}

	retVal = udev_monitor_enable_receiving(mpMonitor);
	if (retVal != 0) {
		LOGD("	ERROR : udev_monitor_enable_receiving() retVal[%d] errno[%d][%s]", retVal, errno, strerror(errno));
		return -1;
	}
	/* Get the file descriptor (fd) for the monitor.
	   This fd will get passed to select() */
	mFd = udev_monitor_get_fd(mpMonitor);

	return mFd;
}
#else
int UsbDeviceMonitor::getFdFromNetlink()
{
	struct sockaddr_nl client;	
	int ret;  
	int buffersize = UEVENT_BUFFER_SIZE;
	mFd = socket(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT);  
	memset(&client, 0, sizeof(client));  
	client.nl_family = AF_NETLINK;	
	client.nl_pid = getpid();  
	client.nl_groups = 1; /* receive broadcast message*/  
	setsockopt(mFd, SOL_SOCKET, SO_RCVBUF, &buffersize, sizeof(buffersize));  
	ret = bind(mFd, (struct sockaddr*)&client, sizeof(client)); 

	LOGD("	NetLink ret[%d] mFd[%d] buffersize[%d]", ret, mFd, buffersize);

	return mFd;
}
#endif
int UsbDeviceMonitor::requestRoleSwitch(int vendorId, int productId, bool bCarPlaySupport)
{
	LOGD("requestRoleSwitch()");

	bool bRsSuccess = false;
	int retVal = 1;
	struct timeval now;
	struct timespec ts;

	LOGD("	wait for role switching");
	gettimeofday(&now, NULL);
	ts.tv_sec = now.tv_sec + 5;
	//ts.tv_sec = now.tv_sec + 30;
	ts.tv_nsec = now.tv_usec * 1000;

	SharedContext* pCtx = SharedContext::getInstance();
	mpRoleSwitcher = new RoleSwitcher(vendorId, productId, bCarPlaySupport);

	pthread_mutex_lock(&pCtx->mMutexRs);
	pCtx->mbRsSuccess = false;
	pthread_cond_timedwait(&pCtx->mCondRs, &pCtx->mMutexRs, &ts);
	bRsSuccess = pCtx->mbRsSuccess;
	retVal = pCtx->retVal;
	pthread_mutex_unlock(&pCtx->mMutexRs);

	delete mpRoleSwitcher;
	mpRoleSwitcher = NULL;
#if !defined(MEEGO)
	if (retVal == ROLESWITCH_SUCCESS && bRsSuccess == true)
		isCarPlayConnected = CARPLAY_CONNECTED;
#endif
	return retVal;
}
				
bool UsbDeviceMonitor::checkCarPlayCapability(int vendorId, int productId)
{
	int rc;
	unsigned char* data;
	bool bCapability = false;
	libusb_context* pUsbCtx;
	libusb_device_handle* pDev;
	
	LOGD("checkCarPlayCapability()");

	data = (unsigned char*)malloc(4);

	rc = libusb_init(&pUsbCtx);
	if (rc < 0) {
		LOGE("Error initializing libusb: rc[%d][%s]", rc, libusb_error_name(rc));
		return false;
	}

	pDev = libusb_open_device_with_vid_pid(pUsbCtx, vendorId, productId);
	if (pDev == NULL) {
		LOGE("device is not exist");
		return false;
	}
	rc = libusb_control_transfer(pDev, 0xc0, 0x53, 0x00, 0x00, data, 4, 0);
	LOGD("data[%u]", *data);
	if (*data == 1) {
		bCapability = true;
	}
	else {
		bCapability = false;
	}
	free(data);

	if (pDev)
		libusb_close(pDev);
	if (pUsbCtx)
		libusb_exit(pUsbCtx);

	return bCapability;

}

bool UsbDeviceMonitor::handleInsertUsb(struct udev_device* pDev, string devNodPath)
{
	ConnectionPath mConnectionPath = CP_NONE;
	
	mConnectionPath = isNormalPath(pDev);
	if (mConnectionPath == CP_NONE) {
		//LOGE("USB PATH is set to CP_NONE, Error");
		return false;
	}
	mConnPath = mConnectionPath;
	LOGI("USB PATH : [%d]", mConnPath);

#if defined(GEELY) || defined(PSA)
	mConnPath = USB_PORT1;
#endif

#ifdef MEEGO
	if (mConnPath == USB_PORT1) {
#endif
		if (mConnectState == LINKCTRL_USB_NOT_CONNECTED) {
			if (isAppleDevice(pDev) == true) {
				bool bCapaCheck = false;
				LOGD("	Apple device is connected devNodPath[%s]!!!", devNodPath.c_str());
				mpUserData = mpConnCbs->getiAP2LinkRunLoopCB(mpConnCbs->pData);

				mConnectState = LINKCTRL_USB_CONNECTED;
				mDevicePath.assign(devNodPath);
				setProductInfo(pDev);

				bCapaCheck = checkCarPlayCapability(mProductInfo.vendorId, mProductInfo.productId);
				LOGI("CarPlay Check[%d]", bCapaCheck);
				if (mpConnCbs && mpConnCbs->connStateCb){
					mpConnCbs->connStateCb(mpConnCbs->pData, LINKCTRL_USB_CONNECTED, mConnPath);
#ifdef ANDROID
					mpConnCbs->deviceInfoCb(mpConnCbs->pData, mProductInfo.productId, mProductInfo.vendorId, strdup(mProductInfo.product.c_str()));
#endif
				}
				else
					LOGE("	ERROR : connStateCb is not registered. mpConnCbs[%p]", mpConnCbs);
				if (bCapaCheck) {
                    if (mpConnCbs && mpConnCbs->carplaySupportCb) {
                        mpConnCbs->carplaySupportCb(mpConnCbs->pData, CCS_CARPLAY_AVAILABLE);
                    }
					return true;
                }
				else {
                    if (mpConnCbs && mpConnCbs->carplaySupportCb) {
                        mpConnCbs->carplaySupportCb(mpConnCbs->pData, CCS_CARPLAY_NOT_AVAILABLE);
                    }
					return false;
                }
			}
			else {
#ifdef MM2014
				LOGD("CarPlay is working && new ipod device is added");
				android_socket(START_IPOD);
				return false;
#endif
			}
		}
		else {
			LOGD("iAP1 Daemon will handle");
#ifdef MM2014
			android_socket(START_IPOD);
#endif
#if defined(ANDROID) || defined(MIBGP)
			mCarPlaySupport = CCS_CARPLAY_NOT_SUPPORTED;
#endif
			return false;
		}
#ifdef MEEGO
	}
#endif
	return false;
}

void UsbDeviceMonitor::StartCarPlay()
{
	if (mConnectState == LINKCTRL_USB_CONNECTED) {
		carplayReady();
	}
	else {
		LOGE("Device is not connected");
	}
}

void UsbDeviceMonitor::carplayReady()
{
	int retVal = 1;
	char* devNodPath = NULL;

	#ifdef KOECN
	#define IAP2UIMODE "persist.sys.iap2"
	char ui_mode[PROPERTY_VALUE_MAX];
	
	property_get(IAP2UIMODE, ui_mode, "");
	LOGD("property[%s]", ui_mode);
	
	if (!strcmp(ui_mode, "true")) {
	#else
	if(mpConnCbs->getCarplayOnOffstateCb(mpConnCbs->pData) == TRUE) {
	#endif
		LOGD(" #######    Carplay ON      #########");
		retVal = requestRoleSwitch(mProductInfo.vendorId, mProductInfo.productId, mbCarPlaySupport);
	}
	else {
		LOGD(" #######    Carplay Off      #########");
	}
	LOGD("	Get the response from Rs retVal[%d] mbCarPlaySupport[%d]", retVal, mbCarPlaySupport);

	if (mpTransport == NULL) {
		if (retVal != ROLESWITCH_SUCCESS) {
			if (retVal == ERROR_IO) {
				LOGE("Device Detached when sending Role Switch Packet");
				return;
			}
			//retVal = ERROR_PIPE
			if (mpConnCbs) {
				if (mpConnCbs->connModeCb)
					mpConnCbs->connModeCb(mpConnCbs->pData, LINKCTRL_AUB_HOST);
				else
					LOGE("	ERROR : mpConnCbs->connModeCb is NULL");

				if (mpConnCbs->carplaySupportCb) {
					mpConnCbs->carplaySupportCb(mpConnCbs->pData, CCS_CARPLAY_NOT_SUPPORTED);
#ifdef MM2014
					android_socket(START_IPOD);
#endif
#if defined(ANDROID) || defined(MIBGP)
					mCarPlaySupport = CCS_CARPLAY_NOT_SUPPORTED;
#endif
					LOGD("   when Host Mode & CarPlay not support, iAP2 sequence is also not checked   ");
					LOGD("   always connect to iAP1 in this case   ");
					return;	
				}
				else
					LOGE("	ERROR : mpConnCbs->carplaySupportCb is NULL");
			}
			else
				LOGE("	ERROR : mpConnCbs is NULL");

		}
		else {
			if (mpConnCbs) {
				if (mpConnCbs->connModeCb)
					mpConnCbs->connModeCb(mpConnCbs->pData, LINKCTRL_AUB_DEVICE);
				else
					LOGE("	ERROR : connModeCb is not registered.");

				if (mbCarPlaySupport == true) {
					if (mpConnCbs && mpConnCbs->carplaySupportCb) {
						mpConnCbs->carplaySupportCb(mpConnCbs->pData, CCS_CARPLAY_SUPPORTED);
#if defined(ANDROID) || defined(MIBGP)
						mCarPlaySupport = CCS_CARPLAY_SUPPORTED;
#endif
					}
					else
						LOGE("	ERROR : carplaySupportCb is not registered. mpConnCbs[%p]", mpConnCbs);
				}

				else {
					if (mpConnCbs && mpConnCbs->carplaySupportCb) {
						mpConnCbs->carplaySupportCb(mpConnCbs->pData, CCS_CARPLAY_NOT_SUPPORTED);
#ifdef MM2014
						android_socket(START_IPOD);
#endif
#if defined(ANDROID) || defined(MIBGP)
						mCarPlaySupport = CCS_CARPLAY_NOT_SUPPORTED;
#endif
						return;
					}
					else
						LOGE("	ERROR : carplaySupportCb is not registered.");
				}
			}
			else
				LOGE("	ERROR : mpConnCbs is NULL");

			devNodPath = "/dev/ttyIAP20";
			mDevicePath.assign(devNodPath);
			mpTransport = new Transport(devNodPath, LINKCTRL_AUB_DEVICE, mpUserData, mpConnCbs);

			if (mpTransport == NULL) {
				for (int k=0; k < 10; k++) {
					mpTransport = new Transport(devNodPath, LINKCTRL_AUB_DEVICE, mpUserData, mpConnCbs);
					if (mpTransport != NULL)
						break;
				}
			}

#ifndef ANDROID
			if (!gAttachedCheck) {
				iAP2LinkRunLoopAttached((iAP2LinkRunLoop_t*)mpUserData);
				gAttachedCheck = true;
			}
#else
			iAP2LinkRunLoopAttached((iAP2LinkRunLoop_t*)mpUserData);
#endif
			return;
		}
	}
	else
		LOGE("	ERROR : mpTransport already created[%p]", mpTransport);
}

#ifdef MM2014
int UsbDeviceMonitor::android_socket(int request)
{
	int i, s, data;
	
	/* socket will not be available until service starts */
	/* try 5 times for reconnection */
	for (i = 0; i < 5; i++) {
		s = socket_local_client("com.windriver.ipod.CarPlay",
		ANDROID_SOCKET_NAMESPACE_ABSTRACT, SOCK_STREAM);
	
		if (s >= 0)
		break;
		/* try again in 1 second */
		sleep(1);
	}

	if (s < 0) {
		LOGD("Failed to connect to iAP1 Daemon");
		return -1;	
	}

	data = START_IPOD;
	if (write(s, (char*)&data, sizeof(data)) < 0)
	{
		LOGD("Fail to send socket data");
		close(s);
		return -1;
	}
	else
		LOGD("Success to send socket data");

	close(s);
	return 0;
}
#endif

void UsbDeviceMonitor::USBModeChange(ConnectionMode UsbMode)
{
	LOGD("################# UsbDeviceMonitor  : USBModeChange##################");
	LOGI("USBModeChange : %d \n", UsbMode);
	#ifdef MEEGO
	bool bDeviceMode = false;
        if ( UsbMode == CM_AUB_HOST) {
                if (mpConnCbs && mpConnCbs->connStateCb){
                        mpConnCbs->connStateCb(mpConnCbs->pData, LINKCTRL_USB_NOT_CONNECTED, CP_NONE);
                }
                else
                        LOGE("  ERROR : connStateCb is not registered. mpConnCbs[%p]", mpConnCbs);

                mConnectState = LINKCTRL_USB_NOT_CONNECTED;
                mDevicePath.clear();
                resetProductInfo();

		if (gAttachedCheck) {
			iAP2LinkRunLoopDetached((iAP2LinkRunLoop_t*)mpUserData);
			gAttachedCheck = false;
		}

                if (mpTransport != NULL) {
                        delete mpTransport;
                        mpTransport = NULL;
                }
                else
                        LOGE("  ERROR : mpTransport already deleted]");
                mConnPath = CP_NONE;
                mpRoleSwitcher->netlinkFunc(CLEAN_REQUEST);
        }
        else if (UsbMode == CM_AUB_DEVICE) {
		bDeviceMode = ToDeviceMode();
		if( bDeviceMode == true ) {
			LOGD("Change to Device mode");
		}
		else {
			LOGD("CarPlay Not Supported");
		}
        }
        else
                LOGE("Undefined Variable");
	#endif //ifdef MEEGO
}

#ifndef ANDROID
void UsbDeviceMonitor::handleInsertCarPlay()
{
	LOGD("ttyIAP2 is added");
}

void UsbDeviceMonitor::handleRemoveCarPlay()
{
	if (mConnectState == LINKCTRL_USB_CONNECTED) {
		LOGD("  Apple device is disconnected !!!");

		if (mpConnCbs && mpConnCbs->connStateCb){
			mpConnCbs->connStateCb(mpConnCbs->pData, LINKCTRL_USB_NOT_CONNECTED, USB_PORT1);
		}
		else
			LOGE("  ERROR : connStateCb is not registered. mpConnCbs[%p]", mpConnCbs);

		mConnectState = LINKCTRL_USB_NOT_CONNECTED;
		mpUserData = mpConnCbs->getiAP2LinkRunLoopCB(mpConnCbs->pData);
		mDevicePath.clear();
		resetProductInfo();

		if (gAttachedCheck) {
			iAP2LinkRunLoopDetached((iAP2LinkRunLoop_t*)mpUserData);
			gAttachedCheck = false;
		}

		if (mpTransport != NULL) {
			delete mpTransport;
			mpTransport = NULL;
		}
		else
			LOGE("  ERROR : mpTransport already deleted");

		mConnPath = CP_NONE;
		mpRoleSwitcher->netlinkFunc(CLEAN_REQUEST);
		LOGD(" ###### netlink CLEAN_UP message sent");
	}
}
#endif

void UsbDeviceMonitor::TerminateDaemon() {

}

}// namespace IAP2
