/*============================================================================
 * COPYRIGHT (c) 2006-2006 by K2L GmbH
 * All rights reserved
 *
 * ABSTRACT: Provides frequently used helper functions and helps interact with
 *			 MAC layer.
 *			 
 *			 
 * GROUP PREFIX: MAC (MOST ABSTRACTION CODE) (group prefix does not apply to C++).
 *=============================================================================        
 *
 * CONTACT:		K2L Software GmbH, Bluecherstr.32, 75177 Pforzheim; info@k2l.de
 *				Tel: 07231/95688-60		Fax: -65	
 *
 *=============================================================================        
 *
 * HISTORY:	
 *
 * Date:		Ver:	Author:	Scope:
 * ----------------------------------------------------------------------------
 * 09.10.2006	1.0		K2L		Initial version
 * 
 *============================================================================*/
#ifndef __NSWCORE_H
#define __NSWCORE_H

#include <IMAC.h>
#include <NSWContainers.h>
#include <K2LMOSTHigh.h>
#include <ETFBlock.h>

#define NSWCORE_LIMIT_MAX_MHP_TX_MESSAGES 25		// [0..254]
#if NSWCORE_LIMIT_MAX_MHP_TX_MESSAGES > 254
#error NSWCORE_LIMIT_MAX_MHP_TX_MESSAGES must be smaller than 255.
#endif

#define NSWCORE_LIMIT_MAX_MHP_RX_MESSAGES 25		// [0..254]
#if NSWCORE_LIMIT_MAX_MHP_RX_MESSAGES > 254
#error NSWCORE_LIMIT_MAX_MHP_RX_MESSAGES must be smaller than 255.
#endif

// Retrieve the real MAC addres from INIC
#define EUI0 0x00u
#define EUI1 0x50u
#define EUI2 0xC2u
#define EUI3 0x6Au
#define EUI4 0xB4u
#define EUI5 0xD5u

class CNetServicesImpl
: public IMACEvents
, public INetServices
{
    typedef CKeyedContainer<uint8_t, CMOSTMessage, NSWCORE_LIMIT_MAX_MHP_TX_MESSAGES > CPendingMhpTxMessages;
    typedef CKeyedContainer<uint8_t, CMOSTMessage, NSWCORE_LIMIT_MAX_MHP_RX_MESSAGES > CPendingMhpRxMessages;

    static const uint16_t MAX_EVENT_LISTENER_COUNT = 10;

    static CNetServicesImpl * m_pTheNetServicesImpl;
    IMAC * m_pMAC;
    struct NetServicesEventListener
    {
        INetServicesEvents* m_pNetServicesEvent;
        bool m_isMAC;
    };
    NetServicesEventListener m_eventListeners[MAX_EVENT_LISTENER_COUNT];
    INetServicesEvents* m_macEventListener;
    uint16_t m_wEventListenersEnd;
    CPendingMhpTxMessages m_pendingMhpTxMessages;
    CPendingMhpRxMessages m_pendingMhpRxMessages;
    CMOSTHighRxBufferConfiguration m_mhpRxBufferConfiguration;
    bool m_bBuffersAvailable;
    bool BufferRequestAllowed();
    CLowLevelServices m_servicesEnabled;
    CETFBlock m_etFBlockService;

public:
    CNetServicesImpl();
    virtual ~CNetServicesImpl();

public:
    //
    // INetServices implementation.
    //
    virtual void Initialize(IMAC *pMAC, const CLowLevelServices &servicesEnabled);
    virtual void Terminate();
    virtual void AddEventListener(INetServicesEvents *pListener);
    virtual void RemoveEventListener(INetServicesEvents *pListener);
    virtual void AddMACEventListener(INetServicesEvents *pListener);
    virtual void RemoveMACEventListener(INetServicesEvents *pListener);
    virtual void EnterNetwork();
    virtual void LeaveNetwork();
    virtual bool SendMessageToMOSTControlChannel(const CMOSTMessage& crMessage);
    virtual bool SendMessageToMOSTAsyncChannel(const CMOSTMessage& crMessage);
    virtual void CyclicService(MILLISECONDS nominalInterval); // Periodic calls; could be useful for extensions.
    virtual void ServiceMOST(); // Intended for call to MostService().
    virtual void ServiceMOSTTimers(); // Intended for call to MostCheckTimers().
    virtual void ServiceInterrupt();
    virtual void InterruptDone();
    virtual void ExtraService();
    virtual uint8_t GetNodePosition();
    virtual DEVICEID GetNodeAddress();
	virtual void SetNodeAddress(DEVICEID nodeAddress);
public:
    //
    // IMACEvents implementation.
    //
    virtual
    void
    OnK2LEvent
    (EVENTCHANNEL channel // [0..0xFE]
     , EVENTID id // [0..0xFF]
     , uint8_t tagLength // [0..3]
     , const uint8_t* cpTagData // ((cpTagData == NULL) && (tagLength == 0)) || (cpTagData != NULL && tagLength >= 0)
     , uint16_t payloadLength
     , const uint8_t* cpPayload
     );

public:
    static void NSWCoreNetServicesRequestMostService();
    static uint8_t NSWCoreCmdRxFilter(Msg_Rx_Type *rx_ptr);
    static void NSWCoreNetServicesAvailable();
    static void NSWCoreNetServicesNotAvailable();
    static void NSWCoreOnTxBufferFreed();
    static void NSWCoreOnLongUnlock();
    static void NSWCoreOnShortUnlock();
    static void NSWCoreOnNetOn();
    static void NSWCoreOnNetOff();
    static void NSWCoreOnMPR(MPR mpr);
    static void NSWCoreOnBandWidthChanged(uint8_t availableQuadletsForPacketData);

    static uint8_t NSWCoreMhpTxStatus(uint8_t status, pTMhpTx tx_ptr);
    static uint8_t * NSWCoreMhpRxBufferAllocator(uint8_t block_type, TMhpRx *msg_ptr);
    static void NSWCoreMhpRxComplete(uint8_t status, pTMhpRx rx_ptr);
    static uint16_t NSWCoreMhpRxGetBufferSize(uint8_t fblockID, uint8_t instanceID, uint16_t functionID, uint8_t operation);

    static uint8_t NSWCoreAMSTxStatus(uint8_t status, TMsgTx *msg_ptr);

private:

    inline void MhpRxRefused(FBLOCKID fblockID, INSTANCEID instanceID, FUNCTIONID functionID, OPTYPE opType)
    {
        if (NULL != m_pMAC)
        {
            const uint8_t CH = EVENT_CHANNEL_MOST_HIGH;
            const uint8_t ID = EVENT_ID_MOST_HIGH_RECEIVE_REFUSED;
            uint8_t Tags[4];
            Tags[0] = fblockID;
            Tags[1] = instanceID;
            uint16_t functionIDAndOpType = functionID;
            functionIDAndOpType <<= 4;
            functionIDAndOpType |= opType & 0x0F;
            Tags[2] = functionIDAndOpType >> 8;
            Tags[3] = functionIDAndOpType & 0x00FF;
            m_pMAC->SendK2LEvent(CH, ID, 4, Tags, 0, NULL);
        }
    }

    inline void MhpRxFailed(FBLOCKID fblockID, INSTANCEID instanceID, FUNCTIONID functionID, OPTYPE opType)
    {
        if (NULL != m_pMAC)
        {
            const uint8_t CH = EVENT_CHANNEL_MOST_HIGH;
            const uint8_t ID = EVENT_ID_MOST_HIGH_RECEIVE_FAILED;
            uint8_t Tags[4];
            Tags[0] = fblockID;
            Tags[1] = instanceID;
            uint16_t functionIDAndOpType = functionID;
            functionIDAndOpType <<= 4;
            functionIDAndOpType |= opType & 0x0F;
            Tags[2] = functionIDAndOpType >> 8;
            Tags[3] = functionIDAndOpType & 0x00FF;
            m_pMAC->SendK2LEvent(CH, ID, 4, Tags, 0, NULL);
        }
    }

private:
    friend struct INetServices;
};

// Implemented in NSW file NetServicesCallbacks.cpp
extern "C" TNetServicesConfig *NetServicesCallbacksInitialize(void);

#endif

