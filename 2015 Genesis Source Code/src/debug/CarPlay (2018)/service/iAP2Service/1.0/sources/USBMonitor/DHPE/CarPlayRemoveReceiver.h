/*
    File:       CarPlayRemoveReceiver.h
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

#ifndef USBREMOVERECEIVER_H_
#define USBREMOVERECEIVER_H_

#include <pthread.h>
#include <string>

#include "UsbDeviceMonitor.h"
#include "utils.h"

using std::string;

namespace IAP2
{
	class UsbDeviceMonitor;
	class CarPlayRemoveReceiver 
	{
		private : 
			int mTid;
			bool mIsTerminated;
			bool mbThreadCreated;
			pthread_cond_t  mCondRecvThSync;
			pthread_mutex_t mMutexRecvThSync;

			//struct UsbConnectCallBack* mpConnCbs;

		public:
			pthread_t mThread;
			void* mpUserData;
			UsbDeviceMonitor* mpDeviceMonitor;

			CarPlayRemoveReceiver(UsbDeviceMonitor* mpUsbDeviceMonitor);
			~CarPlayRemoveReceiver();
			void* recvThread(void* data);
	};

} // namespace IAP2

#endif // #ifdef USBREMOVERECEIVER_H_
