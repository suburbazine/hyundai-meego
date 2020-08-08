/*
    File:       LinkController.h
    Package:    LG iAP2 Service
    Author:     Yongil Park (yongil.park@lge.com)
    Copyright © [2014-2016] by LG Electronics Inc.

    This program or software including the accompanying associated documentation
    (“Software”) is the proprietary software of LG Electronics Inc. and or its
    licensors, and may only be used, duplicated, modified or distributed pursuant
    to the terms and conditions of a separate written license agreement between you
    and LG Electronics Inc. (“Authorized License”). Except as set forth in an
    Authorized License, LG Electronics Inc. grants no license (express or implied),
    rights to use, or waiver of any kind with respect to the Software, and LG
    Electronics Inc. expressly reserves all rights in and to the Software and all
    intellectual property therein. If you have no Authorized License, then you have
    no rights to use the Software in any ways, and should immediately notify LG
    Electronics Inc. and discontinue all use of the Software.
*/

#ifndef LINKCONTROLLER_H_
#define LINKCONTROLLER_H_

#define IAP2SERVICE_EXTERNALACCESSORY
#define iAP2SERVICE_FILETRANSFER
#ifdef ANDROID
#define iAP2SERVICE_FILETRANSFER
#endif

#define _IAP2SERVICE_HANDLE_POWER_FEATURE_IN_IAP2SERVICE

#include <iAP2LinkRunLoop.h>
#include <iAP2FileTransfer.h>

#ifndef MIBGP
#include <UsbDeviceMonitor.h>
#else
#include <USBEventMonitor.h>
#endif

#include "Authentication.h"
#include "Identification.h"
#include "Connection.h"
#include "Power.h"
#include "NowPlaying.h"
#include "DeviceNotification.h"
#include "Telephony.h"
#include "Location.h"
#include "MediaLibraryInformation.h"
#include "MediaLibraryUpdates.h"
#include "HID.h"
#include "VehicleStatus.h"
#include "BTConnection.h"
#include "AssistiveTouch.h"
#include "USBDeviceModeAudio.h"
#include "VoiceOver.h"
#include "WiFiInformation.h"
#include "ExternalAccessoryProtocol.h"
#include "FileTransferManage.h"
#include "FileTransfer.h"
#include "Communications.h"

#include "SendEventToApp.h"
namespace IAP2
{
    //class SendEventToApp;
    class LinkController
    {
        private : 
            int mTid;
            bool mIsTerminated;
            iAP2PacketSYNData_t m_synParam;
            uint8_t m_linkRLBuffer[100*1024];

            uint8_t    mNumSessionInfo;
            iAP2PacketSessionInfo_t mSessionInfo[kIAP2PacketMaxSessions];
            uint8_t            mUserInfo;
            iAP2FileTransfer_t    miAP2FileTransfer_Buffer; 
            Authentication mAuthentication;
            

        public:
            //pthread_t mThread;
            struct UsbConnectCallBack mLikCtrlConnCbs;            
            iAP2LinkRunLoop_t* m_piAP2LinkRunLoop;
            iAP2FileTransfer_t* m_piAP2FileTransfer;
            FileTransferManage mFileTransferManage;
            FileTransfer mFileTransfer;
            SendEventToApp* mpSendEventToApp;            
            Connection mConnection;
            Power mPower;
            NowPlaying mNowPlaying;
#ifndef MIBGP
            UsbDeviceMonitor* mpDevMonitor;
#else
            USBEventMonitor* mpDevMonitor;
#endif
            DeviceNotification mDeviceNotification;
            Identification mIdentification;
            Location mLocation;
            Telephony mTelephony;
            MediaLibraryInformation mMediaLibraryInformation;
            MediaLibraryUpdates mMediaLibraryUpdates;
            HID mHID;
            VehicleStatus mVehicleStatus;
            BTConnection mBTConnection;    
            AssistiveTouch mAssistiveTouch;            
            USBDeviceModeAudio mUSBDeviceModeAudio;
            VoiceOver mVoiceOver;
            WiFiInformation mWiFiInformation;
            ExternalAccessoryProtocol mExternalAccessoryProtocol;         
            Communications mCommunications;
           
            
        private:
            void initSynParam(iAP2PacketSYNData_t &synParam);
        
            static void sendDetectCB_Caller(struct iAP2Link_st* link, BOOL bBad);
            static void sendPacketCB_Caller(struct iAP2Link_st* link, iAP2Packet_t* packet);
            static BOOL recvDataCB_Caller(struct iAP2Link_st* link, uint8_t* data, uint32_t dataLen, uint8_t session);
            static void connectedCB_Caller(struct iAP2Link_st* link, BOOL bConnected);
            void handleReceivedData(iAP2Link_st* link, uint8_t* data, uint32_t dataLen, uint8_t session);
            
            void handleFileTransferReceivedData(iAP2FileTransfer_st* fileXfer, void* userInfo);

            void handleEAPReceivedData(iAP2Link_st* link, uint8_t* data, uint32_t dataLen, uint8_t session);

        public:
            LinkController(SendEventToApp* pSendEventToApp);
            ~LinkController();
            void createIAP2LinkRunLoopForAccessory();
            //void* monitorThread(void* data);
            void DeleteiAP2LinkRunLoop();
            
            void sendDetectCB(struct iAP2Link_st* link, BOOL bBad);
            void sendPacketCB(struct iAP2Link_st* link, iAP2Packet_t* packet);
            BOOL recvDataCB(struct iAP2Link_st* link, uint8_t* data, uint32_t dataLen, uint8_t session);
            void connectedCB(struct iAP2Link_st* link, BOOL bConnected);

            void processOutBoundTraffic(struct iAP2Link_st* link, uint8_t session, ByteArray& rSendPck);
            void processOutBoundTrafficEAPData(struct iAP2Link_st* link, uint8_t session, uint8_t* pdata, uint32_t size);
            uint8_t getSessionId(iAP2PacketServiceType_t sessionType);

            void usbConnStateCb(UsbConnectState connState, ConnectionPath connPath);
            void usbConnModeCb(AccUsbMode usbMode);
            void carPlaySupportCb(ConnectionCarPlaySupport carPlaySupport);
            void iap2SupportCb(ConnectionIap2Support iap2Support);
#ifdef ANDROID
            void usbDeviceInfoCb(int productId, int vendorId, char *product);
#endif
            //void createIAP2FileTransferCreate(uint8_t bufferID);
            BOOL recvFileTransferDataCB(struct iAP2FileTransfer_st* fileXfer, void* userInfo);
    };

} // namespace IAP2


#endif // #ifndef LINKCONTROLLER_H_

