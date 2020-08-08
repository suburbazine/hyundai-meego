/*
    File:       UsbDeviceMonitor.h
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

#ifndef USBDEVICEMONITOR_H_
#define USBDEVICEMONITOR_H_

#include <pthread.h>
#include <libudev.h>
#include <string>

#ifndef ANDROID
#include "CarPlayRemoveReceiver.h"
#endif
#include "Transport.h"
#include "RoleSwitcher.h"
#include "iAp2AppFrameworkExternal.h"
#include "iAp2AppFrameworkExternal_command.h"
#include "iAp2AppFrameworkExternal_event.h"

using std::string;

#if !defined(MEEGO)
#define START_IPOD 1

#define CARPLAY_NOT_CONNECTED	0
#define CARPLAY_CONNECTED	1
#endif

namespace IAP2
{
	typedef void (*UsbConnectionStateUpdate_f)(void* pData, UsbConnectState connState, ConnectionPath connPath);
	typedef void (*UsbConnectionModeUpdate_f)(void* pData, AccUsbMode usbMode);
	typedef void (*CarplaySupportUpdate_f)(void* pData, ConnectionCarPlaySupport carPlaySupport);
	typedef void (*Iap2SupportUpdate_f)(void* pData, ConnectionIap2Support iap2Support);
	#ifdef ANDROID
	typedef void (*UsbDevieInformationUpdate_f)(void* pData, int productId, int vendorId, char *product);
	#endif
	typedef BOOL (*getCarplayOnOffStateFunc_f)(void* pData);
	typedef BOOL (*getCarPlayClientStateFunc_f)(void* pData);
	typedef void (*createIAP2LinkRunLoopForAccessory_f)(void* pData);
	typedef void (*DeleteiAP2LinkRunLoop_f)(void* pData);
	typedef iAP2LinkRunLoop_t*  (*getiAP2LinkRunLoop_f)(void* pData);
	
	struct UsbConnectCallBack {
		void* pData; // User Data
		UsbConnectionStateUpdate_f connStateCb;
		UsbConnectionModeUpdate_f connModeCb;
		CarplaySupportUpdate_f carplaySupportCb;
		Iap2SupportUpdate_f iap2SupportCb;
		#ifdef ANDROID
		UsbDevieInformationUpdate_f deviceInfoCb;
		#endif
		getCarplayOnOffStateFunc_f getCarplayOnOffstateCb;
		getCarPlayClientStateFunc_f getCarPlayClientStateCb;
		createIAP2LinkRunLoopForAccessory_f createIAP2LinkRunLoopForAccessoryCb;
		DeleteiAP2LinkRunLoop_f DeleteiAP2LinkRunLoopCb;
		getiAP2LinkRunLoop_f getiAP2LinkRunLoopCB;
	};

	struct ProductInfo	 {
		int vendorId;
		int productId;
		string manufacturer;
		string product;
		string serial;
	};
#ifndef ANDROID
	class CarPlayRemoveReceiver;
#endif
	class UsbDeviceMonitor
	{
		private : 
			int mTid;
			bool mIsTerminated;

			string mDevicePath;
			UsbConnectState mConnectState;
			ConnectionPath mConnPath;

			struct udev* mpUdev;
#if defined(MEEGO)
			struct udev_monitor* mpMonitor;
#endif // #ifdef MEEGO
			int mFd;
#if !defined(MEEGO)
			int mCarPlaySupport;
			int isCarPlayConnected;
#endif
			struct ProductInfo mProductInfo;
			struct UsbConnectCallBack* mpConnCbs;

			bool mbCarPlaySupport;
#ifndef ANDROID
			CarPlayRemoveReceiver* mpCarPlayRemoveReceiver;
#endif
			RoleSwitcher* mpRoleSwitcher;

			// This condition and mutex is used for monitor thread(monitorThread).
			pthread_cond_t mCondMonThSync;
			pthread_mutex_t mMutexMonThSync;

		public:
			pthread_t mThread;
			Transport* mpTransport;
#ifndef ANDROID
			bool gAttachedCheck;
#endif
			void* mpUserData;

		private:
			bool ToDeviceMode();
			bool setUpMonitoringUsb();
#if defined(MEEGO)
			int getFdFromUdev();
#else
			int getFdFromNetlink();
#endif	
#ifdef ANDROID
			struct udev_device* monitorReceiveDevice(char** ppAction, bool *isIpod);
#endif // #ifdef ANDROID 
			bool isAppleDevice(struct udev_device* pDev);
			ConnectionPath isNormalPath(struct udev_device* pDev);
			void setProductInfo(struct udev_device* pDev);
			void resetProductInfo();

			void usbConnStateCb(UsbConnectState connState, ConnectionPath connPath);
			void usbConnModeCb(AccUsbMode usbMode);

		public:
			UsbDeviceMonitor( struct UsbConnectCallBack* pCb);
			~UsbDeviceMonitor();
			void* monitorThread(void* data);
			int requestRoleSwitch(int vendorId, int productId, bool bCarPlaySupport);
			bool checkCarPlayCapability(int vendorId, int productId);
			bool handleInsertUsb(struct udev_device* pDev, string devNodPath);
			void StartCarPlay();
			void carplayReady();
			#ifdef MM2014
			int android_socket(int request);
			#endif
			void USBModeChange(ConnectionMode UsbMode);
			void handleInsertCarPlay();
			void handleRemoveCarPlay();
            void TerminateDaemon();
	};

} // namespace IAP2

#endif // #ifdef USBDEVICEMONITOR_H_
