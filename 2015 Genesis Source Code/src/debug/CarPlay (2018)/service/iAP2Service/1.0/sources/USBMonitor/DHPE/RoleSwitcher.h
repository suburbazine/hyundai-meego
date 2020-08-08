/*
    File:       RoleSwitcher.h
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

#ifndef ROLESWITCHER_H_
#define ROLESWITCHER_H_

#include <pthread.h>
#include <sys/ioctl.h>

#include "libusb.h"
#include "Transport.h"

using std::string;

#ifdef MEEGO
#define NETLINK_TEST	31
#define MAX_NLMSG_LEN	1024

#define DEVICE_MODE     0
#define CLEAN_REQUEST   4

#define ROLE_SWAP_REQUEST	0x01
#define ROLE_SWAP_COMPLETE	0x02
#define CARPLAY_DETACHED	0x03
#define CLEAN_UP_REQUEST	0x04
#define CLEAN_UP_COMPLETE	0x05

#define FRONT_SEAT_USB_NUM 	1
#define REAR_SEAT_USB_NUM 	2	
#endif // #ifdef MEEGO

#ifdef GEELY
#define IMX_USB1_MODE_IS_DEVICE        0x10
#define IMX_USB2_MODE_IS_DEVICE        0x20
#define IMX_USB1_MODE_IS_HOST          0x11
#define IMX_USB2_MODE_IS_HOST          0x21
#define IMX_USB_MODE_ALL_HOST          0xA1
#endif



namespace IAP2
{
	class RoleSwitcher
	{
		public:
			pthread_t mThread;

		private : 
			int mTid;

			int mVendorId;
			int mProductId;
			bool mbCarPlaySupport;

			struct libusb_device_handle* mphDev;
			libusb_context*  mpUsbCtx;

		public:
			#ifdef ANDROID
			int restore_usb_config();
			#endif
			RoleSwitcher(int vendorId, int productId, bool bCarPlaySupport);
			~RoleSwitcher();
			void* roleSwitcherThread(void* data);

			#ifdef MEEGO
            		int netlinkFunc(int request);
			libusb_device_handle* open_device_with_bus_num(libusb_context* mCtx);
			#endif //#ifdef MEEGO

		private:
			int startRoleSwitch(bool bCarPlaySupport);
			int sendRoleSwitchPacket(struct libusb_device_handle* phDev, bool bCarPlaySupport);
	};

} // namespace IAP2

#endif // #ifdef ROLESWITCHER_H_

