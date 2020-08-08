/*
    File:       Identification.h
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

#ifndef IDENTIFICATION_H_
#define IDENTIFICATION_H_

#include "iAP2LinkRunLoop.h"
#include "DataDecoder.h"
#include "DataEncoder.h"
#include "Transport.h"

namespace IAP2
{
    class LinkController;
    
    struct IdentificationInformation{        
        char Name[50]; 
        char ModelIdentifier[50]; 
        char Manufacturer[50]; 
        char SerialNumber[50]; 
        char FirmwareVersion[50]; 
        char HardwareVersion[50]; 
        char CurrentLanguage[50]; 
    };
    
    class Identification
    {
        public:
            uint8_t CountRetryIdentfication;
        public:
            Identification();
            ~Identification();

            void sendIdentificationInformation(LinkController* pLnkCtrler, struct iAP2Link_st* link, uint8_t session, IdentificationRejectedEvt* pEvent);
            void sendCancelIdentification(LinkController* pLnkCtrler, struct iAP2Link_st* link, uint8_t session);
            void handleIdentificationRejected(LinkController* pLnkCtrler, struct iAP2Link_st* link, int8_t session, uint8_t* pData);
            IdentficationMsgSelectArg mIdentficationMsgSelect;
            struct IdentificationInformation mIdentificationInformation;
            
        private:
            void createIdentificationInformationParamList(LinkController* pLnkCtrler, Transport* pTransport, IdentificationRejectedEvt* pEvent, std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList);            
            void createIdentificationInformationParamList_MsgRecv(LinkController* pLnkCtrler, IdentificationRejectedEvt* pEvent, Transport* pTransport, std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList);
            void createIdentificationInformationParamList_MsgSent(LinkController* pLnkCtrler, IdentificationRejectedEvt* pEvent, Transport* pTransport, std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList);
            void decodeIdentifiCationRejected(uint8_t* pData, IdentificationRejectedEvt* pEvt);
            void createIdentificaitonInformationProductParamList(IdentificationRejectedEvt* pEvent, std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList);
            uint32_t createGroupParamBytesForUsbDeviceTransportComponent(uint8_t** encodedBytes);
            uint32_t createGroupParamBytesForUsbHostTransportComponent(uint8_t** encodedBytes);
            uint32_t createGroupParamBytesForIap2HidComponent(uint8_t** encodedBytes);
            uint32_t createGroupParamBytesForBTTransportComponent(uint8_t** encodedBytes);
            uint32_t createGroupParamBytesForVehicleInformationComponent(uint8_t** encodedBytes);
            uint32_t createGroupParamBytesForVehicleStatusComponent(uint8_t** encodedBytes);
            uint32_t createGroupParamBytesForLocationInformationComponent(uint8_t** encodedBytes);
            uint32_t createGroupParamBytesForSupportedExternalAccessroyProtocolComponent(uint8_t** encodedBytes);
            
            void createUsbDeviceTransportComponentParamList(std::vector<int>& nParamIdList, std::vector<const uint8_t *>& paramDataList, std::vector<uint32_t>& nParamDataLenList);
            void createUsbHostTransportComponentParamList(std::vector<int>& nParamIdList, std::vector<const uint8_t *>& paramDataList, std::vector<uint32_t>& nParamDataLenList);
            void createiAp2HidComponentParamList(std::vector<int>& nParamIdList, std::vector<const uint8_t*>& paramDataList, std::vector<uint32_t>& nParamDataLenList);
            void createBTTransportComponentParamList(std::vector<int>& nParamIdList, std::vector<const uint8_t*>& paramDataList, std::vector<uint32_t>& nParamDataLenList);
            void createVehicleInformationComponentParamList(std::vector<int>& nParamIdList, std::vector<const uint8_t*>& paramDataList, std::vector<uint32_t>& nParamDataLenList);
            void createVehicleStatusComponentParamList(std::vector<int>& nParamIdList, std::vector<const uint8_t*>& paramDataList, std::vector<uint32_t>& nParamDataLenList);
            void createLocationInformationComponentParamList(std::vector<int>& nParamIdList, std::vector<const uint8_t*>& paramDataList, std::vector<uint32_t>& nParamDataLenList);            
            void createCancelIdentificationParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList);
            void createSupportedExternalAccessroyProtocolParamList(std::vector<int>& nParamIdList, std::vector<const uint8_t*>& paramDataList, std::vector<uint32_t>& nParamDataLenList);    
            void createCurrentSupportLanguage(IdentificationRejectedEvt* pEvent, CmdSaveUtf8Param supportedLanguage ,std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList);
            int GetProductInformationFromFilePath();
            BOOL CheckProductInformationValue(char* pValue);

            void parsingMessagesByAccessory(IdentificationRejectedEvt* pEvt, uint8_t* pMessageIdentifier, uint32_t CountIdentifier);            
            void SetParameterValue(char* pParameterName, char* pParameterValue);

        public :
            int GetIdentificationInformation();
            int handleIdentificationFailedUpdateEvent(LinkController* pLnkCtrler);
        
    };

} // namespace IAP2

#endif // #ifndef IDENTIFICATION_H_
