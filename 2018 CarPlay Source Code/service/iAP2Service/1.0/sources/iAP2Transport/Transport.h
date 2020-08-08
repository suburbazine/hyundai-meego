/*
    File:       Transport.h
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

#ifndef TRANSPORT_H_
#define TRANSPORT_H_

#include <pthread.h>
#include <string>
#include <termios.h>

#include "ByteArray.h"
#include "iAP2LinkRunLoop.h"

using std::string;

namespace IAP2
{
	#define  MAX_RCV_PACKET_SIZE	(66000) // IAP2 spec L:ik playload 65535 + Link + usb header.
	//#define  MAX_RCV_PACKET_SIZE	(4096)

	enum UsbConnectState {
		LINKCTRL_USB_NOT_CONNECTED	= 0,
		LINKCTRL_USB_CONNECTED		= 1
	};

	enum AccUsbMode {
		LINKCTRL_AUB_HOST	= 0,
		LINKCTRL_AUB_DEVICE	= 1
	};
	
	class UsbDeviceMonitor;
	class Transport
	{
		private : 
			int mTid;
			bool mIsTerminated;
			bool mbThreadCreated;

			string mDevicePath;

			int mReaderFd;
			int mWriterFD;
			void* mpUserData;
			struct UsbConnectCallBack* mpConnCbs;
			ByteArray mRcvPacketRepo;

			iAP2Packet_t* mpRcvPacket;
			bool mbRcbPacketIsComplete;
            uint32_t nbeforeLength;

			struct termios mOrigReadTty;
			struct termios mOrigWriteTty;

			bool mbSendIap2Sequene;

			// This condition and mutex is used for receiver thread(recieverThread).
			pthread_cond_t mCondRecvThSync;
			pthread_mutex_t mMutexRecvThSync;

		public:
			pthread_t mThread;
			AccUsbMode mAccUSbMode;

		private:
			int sendHidData(ByteArray& rSendPacket);
			int sendBulkData(ByteArray& rSendPacket);
			//int sendBulkData(unsigned char* pData, int dataLen);

			void assembleRcvPacket(uint8_t* pRcvHidPaket, uint32_t length);
			void processInBoundTraffic(uint8_t* pRcvPaket, uint32_t length);

		public:
			Transport(string devPath, AccUsbMode accUSbMode, void* pUserData, struct UsbConnectCallBack* pCb);
			~Transport();
			void* recieverThread(void* data);
			int sendDataToDevice(ByteArray& rSendPacket, bool bIap2Sequene);
	};

} // namespace IAP2

#endif // #ifdef TRANSPORT_H_

