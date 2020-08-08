//Warning disabled because of following arrays:
//-CNetServicesImpl::m_eventListeners
//It is only a tip from compiler that array initialization behavior was changed.
//It has no impact on our code.
#pragma warning( disable : 4351 )

#include <new>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
extern "C"
{
#include <adjust1.h>
#include <adjust2.h>
#include <adjustmh.h>
#include <mostns.h>
}
#include <NSWDebug.h>
#include <NSWCore.h>
#include <LLDStub.h>
#include <K2LMOSTHigh.h>
#include <K2LDebug.h>
#include <MOSTDebugZones.h>
#include <K2LSystem.h>

#define BENCH 0

#define ACS_AMF_BRIDGE_MESSAGE_CHANNEL 0xFE
#define ACS_AMF_BRIDGE_TX_MESSAGE_ID   0x01
#define ACS_AMF_BRIDGE_RX_MESSAGE_ID   0x02
#define ACS_AMF_BRIDGE_NET_STATE       0x03
#define ACS_AMF_BRIDGE_INIC_MACADDRESS 0x04

static const uint32_t MOSTSERVICE_TIME_LIMIT = 20;
static const uint32_t LLDSTUBSERVICEINTERRUPT_TIME_LIMIT = 20;

static uint8_t s_netState = 0;
static uint16_t s_deviceId = 0x100;

CNetServicesImpl::CNetServicesImpl
(
 )
: IMACEvents()
, INetServices()
, m_pMAC(NULL)
, m_eventListeners()
, m_macEventListener(NULL)
, m_wEventListenersEnd(0)
, m_pendingMhpTxMessages()
, m_pendingMhpRxMessages()
, m_mhpRxBufferConfiguration()
, m_bBuffersAvailable(true)
, m_servicesEnabled( CLowLevelServices() )
, m_etFBlockService()
{
    for (uint32_t index = 0; index < MAX_EVENT_LISTENER_COUNT; index++)
    {
        m_eventListeners[index].m_pNetServicesEvent = NULL;
        m_eventListeners[index].m_isMAC = false;
    }
}
//virtual
CNetServicesImpl::~CNetServicesImpl
(
 )
{
    m_pendingMhpTxMessages.clear();
}

//static
CNetServicesImpl *
        CNetServicesImpl::m_pTheNetServicesImpl = NULL;

//static
INetServices *
INetServices::GetInstance
(
 )
{
    if (NULL == CNetServicesImpl::m_pTheNetServicesImpl)
    {
        CNetServicesImpl::m_pTheNetServicesImpl = new (std::nothrow) CNetServicesImpl();
    }
    K2L_DEBUGCHK(NULL != CNetServicesImpl::m_pTheNetServicesImpl);
    return CNetServicesImpl::m_pTheNetServicesImpl;
}

/*virtual */
void
CNetServicesImpl::Initialize
(IMAC *pMAC,
 const CLowLevelServices &servicesEnabled
 )
{   
    m_servicesEnabled = servicesEnabled;

    K2L_DEBUGCHK(NULL != pMAC);
    m_pMAC = pMAC;

    // So we can receive MOST high configuration and Sync channel administration events.
    m_pMAC->AddEventListener(this);
    
    if ( m_servicesEnabled.mostHighSocketManager )
    {
        m_mhpRxBufferConfiguration.Initialize(pMAC);
    }
    m_etFBlockService.Initialize(pMAC, INetServices::GetInstance());
    
    m_pMAC->JoinChannel(ACS_AMF_BRIDGE_MESSAGE_CHANNEL);
}

/*virtual */
void
CNetServicesImpl::Terminate
(
 )
{
    K2L_DEBUGCHK(NULL != m_pMAC);
    if (NULL != m_pMAC)
    {
        if ( m_servicesEnabled.mostHighSocketManager )
        {
            m_mhpRxBufferConfiguration.Terminate();
        }
        m_etFBlockService.Terminate();

        m_pMAC->RemoveEventListener(this);
        m_pMAC = NULL;
    }
}

/*virtual */
void
CNetServicesImpl::AddEventListener(INetServicesEvents *pListener)
{
    K2L_DEBUGCHK(NULL != pListener);
    if (NULL != pListener)
    {
        if (m_wEventListenersEnd < MAX_EVENT_LISTENER_COUNT)
        {
            m_eventListeners[m_wEventListenersEnd].m_pNetServicesEvent = pListener;
            m_eventListeners[m_wEventListenersEnd].m_isMAC = false;
            m_wEventListenersEnd++;
        }
    }
}

/*virtual */
void
CNetServicesImpl::RemoveEventListener(INetServicesEvents *pListener)
{
    uint16_t wIndex;
    for (wIndex = 0; wIndex < m_wEventListenersEnd; wIndex++)
    {
        if (pListener == m_eventListeners[wIndex].m_pNetServicesEvent)
        {
            m_eventListeners[wIndex].m_pNetServicesEvent = NULL;
            m_eventListeners[wIndex].m_isMAC = false;
            break;
        }
    }
    if (wIndex < m_wEventListenersEnd)
    { // We found a listener and will move subsequent listeners one place towards the start of the array.
        for (/* empty */; wIndex < (m_wEventListenersEnd - 1); wIndex++)
        {
            m_eventListeners[wIndex] = m_eventListeners[wIndex + 1];
        }
        m_wEventListenersEnd--; // We removed 1 and relocated the others...
    }
}


/*virtual*/ 
void 
CNetServicesImpl::AddMACEventListener(INetServicesEvents *pListener)
{
    K2L_DEBUGCHK(NULL != pListener);
	if (NULL != pListener)
	{
		if (m_wEventListenersEnd < MAX_EVENT_LISTENER_COUNT)
		{
                    m_eventListeners[m_wEventListenersEnd].m_pNetServicesEvent = pListener;
                    m_eventListeners[m_wEventListenersEnd].m_isMAC = true;
                    m_macEventListener = pListener;
		    m_wEventListenersEnd++;
		}
	}
}

/*virtual */
void
CNetServicesImpl::RemoveMACEventListener(INetServicesEvents *pListener)
{
    //Do not remove pListener parameter because of backward compatibility with RemoveEventListener().
    RemoveEventListener(pListener);
    m_macEventListener = NULL;
}

/*virtual */
void
CNetServicesImpl::EnterNetwork
(
 )
{
    TNetServicesConfig *pMNSConfig = NetServicesCallbacksInitialize();

    // TODO: Remove watchdog disable code. (or remove the TODO).
#ifdef DEBUG
    pMNSConfig->general.watchdogmode.overwrite = TRUE;
    pMNSConfig->general.watchdogmode.reset = FALSE;
    pMNSConfig->general.watchdogmode.timeout = MNS_TM_WD_DISABLE;
    pMNSConfig->general.watchdogmode.auto_shutdown_delay = 0xFFFF;
#endif

    LLDStubResult_t result = LLDStubInitialize(m_pMAC, &pMNSConfig->lld);
    //K2L_DEBUGCHK( LLDSTUB_RESULT_OK == result );
#if defined K2LRELEASE || defined K2LDEBUG
    if ((LLDSTUB_RESULT_OK != result))
    {
        K2L_RETAILMSG(K2L_ZONE_ERROR, (K2LDEBUG_TEXT("LLDStubInitialize() failed.\r\n")));
    }
#endif
    if (LLDSTUB_RESULT_OK == result)
    {
        InitNetServices();
    }
}

/*virtual */
void
CNetServicesImpl::LeaveNetwork
(
 )
{
    LLDStubTerminate();
}

/*virtual */
bool
CNetServicesImpl::SendMessageToMOSTControlChannel
(const CMOSTMessage& crMessage
 )
{
    // TODO: Either here or in MAC, BROADCAST and internal Addressing might be missing.
#ifndef PACKET_COMMUNICATION_ONLY
    
    K2L_DEBUGCHK(MOSTMESSAGE_TRANSPORT_CONTROL_CHANNEL == crMessage.Transport());
    bool bSent = false;

    if (m_bBuffersAvailable)
    {
        TMsgTx * pTxMessage = MsgGetTxPtrExt(crMessage.PayloadLength());
        if (NULL != pTxMessage)
        {
            pTxMessage->Tgt_Adr = crMessage.DeviceID();
            pTxMessage->FBlock_ID = crMessage.FBlockID();
            pTxMessage->Inst_ID = crMessage.InstanceID();
            pTxMessage->Func_ID = crMessage.FunctionID();
            pTxMessage->Operation = crMessage.OpType();
            pTxMessage->Length = crMessage.PayloadLength();

            if ((0 < pTxMessage->Length) && (NULL != pTxMessage->Data))
            {
                const uint8_t *cpucPayload = crMessage.Payload();
                K2L_DEBUGCHK(NULL != cpucPayload);
                memcpy(pTxMessage->Data, cpucPayload, pTxMessage->Length);
            }
			MsgSend(pTxMessage);
			bSent = true;
        }
        else
        {
            m_bBuffersAvailable = false;
        }
    }
    return bSent;
#else
    return false;
#endif
}

//static
void
CNetServicesImpl::NSWCoreOnTxBufferFreed
(void
 )
{
    if (NULL != m_pTheNetServicesImpl)
    {
        m_pTheNetServicesImpl->m_bBuffersAvailable = true;
    }
}

/*virtual */
bool
CNetServicesImpl::SendMessageToMOSTAsyncChannel
(const CMOSTMessage& crMessage
 )
{
#if MAX_MHP_TX_HANDLE < 1
#error You have to define MAX_MHP_TX_HANDLE in adjustmh.h to at least 1 byte to meet the requirements of this implementation.
#endif

    bool bSent = false;
    TMhpCon mhpConnection;
    TMhpTx *pOutMsg = NULL;

    if( 0 < crMessage.PayloadLength() )
    {
        if( !m_pendingMhpTxMessages.full() )
        {
            const uint8_t *cpucPayload = crMessage.Payload();
            K2L_DEBUGCHK(NULL != cpucPayload);
            if( NULL != cpucPayload)
            {
            mhpConnection.Addr = crMessage.DeviceID();
            mhpConnection.FBlock_ID = crMessage.FBlockID();
            mhpConnection.Inst_ID = crMessage.InstanceID();
            mhpConnection.Func_ID = crMessage.FunctionID();
            mhpConnection.Operation = crMessage.OpType();

            pOutMsg = MhpGetTxPtrCon( &mhpConnection );
            if( NULL != pOutMsg )
            {
                CPendingMhpTxMessages::key_type key = m_pendingMhpTxMessages.add( crMessage );
                K2L_DEBUGCHK(m_pendingMhpTxMessages.capacity != key );

                //Following if condition is only needed for propert ET functionality.
                //Please note that this adds some minor runtime costs. If you
                //don't necessary need this you can turn this check off and set
                //options to 1 which means BlockAck. Basically you only need FrameAck
                //for ET functionality (this is not used in any other case).
                if (m_etFBlockService.UseSingleFrameAck(crMessage))
                {
                    pOutMsg->Options = 0x02;
                }
                else
                {
                    pOutMsg->Options = 0x01;
                }
                
                pOutMsg->Priority = 1;
#ifdef MNSV3
                pOutMsg->TotalPacketLength = crMessage.TotalMessageLength();
#endif
                pOutMsg->Length = crMessage.PayloadLength();
                pOutMsg->DataPtr = const_cast<uint8_t*>(cpucPayload); 
                pOutMsg->TxHandle[0] = key;
                
                if ( MHP_SEND_SUCCESS == MhpSend(pOutMsg) )
                {
                    bSent = true;
                }
                else
                {
                    m_pendingMhpTxMessages.remove(key);
                }
            }
        }
            else
            {
                K2L_RETAILMSG(K2L_ZONE_ERROR, (K2LDEBUG_TEXT("CNetServicesImpl::SendMessageToMOSTAsyncChannel() - cpucPayload == NULL\r\n")));
            }
        }
    }
    else
    {    // Cannot send 0 bytes of payload... discard this message.
        bSent = true;
    }

    return bSent;
}

/*virtual */
void
CNetServicesImpl::CyclicService
(MILLISECONDS nominalInterval
 )
{
    // This tick is sometimes nice to see but in general of very reduced value. Only activate if in doubt if NSW is functional at all.
    //static uint32_t dwCounter = 0;
    //++dwCounter;
    //if( (dwCounter % 10) == 0 )
    //{
    //	K2L_RETAILMSG(ZONE_NSW,(K2LDEBUG_TEXT("TICK")));
    //}
	m_etFBlockService.Process(nominalInterval);
}

//virtual
void
CNetServicesImpl::ServiceMOST
(
 )
{
    uint32_t startTime = k2l::platform::system::SystemGetTickCount();

    MostService();

    uint32_t endTime = k2l::platform::system::SystemGetTickCount();
    uint32_t diff = endTime - startTime;
    if (MOSTSERVICE_TIME_LIMIT < diff)
    {
        K2L_DEBUGMSG(K2L_ZONE_INFO, (K2LDEBUG_TEXT("BENCH: MostService() took %d ms"), diff));
    }
}

//virtual
void CNetServicesImpl::ServiceMOSTTimers
(
 )
{
    MostCheckTimers();
}

/*virtual */
void
CNetServicesImpl::ServiceInterrupt
(
 )
{
    uint32_t startTime = k2l::platform::system::SystemGetTickCount();
    LLDStubServiceInterrupt();
    uint32_t endTime = k2l::platform::system::SystemGetTickCount();
    uint32_t diff = endTime - startTime;
    if (LLDSTUBSERVICEINTERRUPT_TIME_LIMIT < diff)
    {
        K2L_DEBUGMSG(K2L_ZONE_INFO, (K2LDEBUG_TEXT("BENCH: LLDStubServiceInterrupt() took %d ms"), diff));
    }
}

/*virtual */
void
CNetServicesImpl::InterruptDone
(
 )
{
    LLDStubInterruptDone();
}

/*virtual */
void
CNetServicesImpl::ExtraService
(
 )
{
}

/*virtual */
uint8_t
CNetServicesImpl::GetNodePosition
(
 )
{
#ifndef PACKET_COMMUNICATION_ONLY
    return MostGetNodePos();
#else
    return 0;
#endif
}

/*virtual */
DEVICEID
CNetServicesImpl::GetNodeAddress
(
 )
{
#ifndef PACKET_COMMUNICATION_ONLY
    return MostGetNodeAdr();
#else
    return s_deviceId;
#endif
}

/*virtual*/
void
CNetServicesImpl::SetNodeAddress(DEVICEID nodeAddress)
{
#ifndef PACKET_COMMUNICATION_ONLY
	MostSetNodeAdr(nodeAddress);
#endif
}

//virtual
void
CNetServicesImpl::OnK2LEvent
(EVENTCHANNEL channel // [0..0xFE]
 , EVENTID id // [0..0xFF]
 , uint8_t tagLength // [0..3]
 , const uint8_t* cpTagData // ((cpTagData == NULL) && (tagLength == 0)) || (cpTagData != NULL && tagLength >= 0)
 , uint16_t payloadLength
 , const uint8_t* cpPayload
 )
{
    if (ACS_AMF_BRIDGE_MESSAGE_CHANNEL == channel)
    {
        if (ACS_AMF_BRIDGE_NET_STATE == id)
        {
            if (payloadLength >= 1)
            {
                uint8_t newNetState = cpPayload[0];

                if (0x01 == newNetState && 0x00 == s_netState)
                {
					if (payloadLength >= 3)
					{
						s_deviceId = cpPayload[1] << 8 | cpPayload[2];
					}
				
                    NSWCoreOnNetOn();
                    MhpInit();
                }
                else if (0x00 == newNetState && 0x01 == s_netState)
                {
                    NSWCoreOnNetOff();
                    MhpTxEndAllCon();
                }

                uint8_t data[1] = { newNetState };

                Msg_Rx_Type rx_ptr;
                rx_ptr.FBlock_ID = 0x02; // NWM
                rx_ptr.Inst_ID   = 0x01;
                rx_ptr.Func_ID   = 0xA00; // Configuration
                rx_ptr.Operation = 0x0C; // Status
                rx_ptr.Src_Adr   = 0xFFFF;
                rx_ptr.Length    = 1;
                rx_ptr.Data      = data;
                rx_ptr.Rcv_Type  = MSG_RCV_TYPE_LOGICAL;

                CNetServicesImpl::NSWCoreCmdRxFilter(&rx_ptr);

                s_netState = newNetState;
            }
        }
        else
        if (ACS_AMF_BRIDGE_INIC_MACADDRESS == id)
        {
            if (payloadLength >= 6)
            {
                LLDSetMostNetEUI48(cpPayload);
            }
        }
    }
}



//static
void
CNetServicesImpl::NSWCoreNetServicesRequestMostService
(
 )
{
    if (NULL != m_pTheNetServicesImpl)
    {
        if (NULL != m_pTheNetServicesImpl->m_pMAC)
        {
            m_pTheNetServicesImpl->m_pMAC->RequestMOSTService();
        }
    }
}


// Constants, we found in cmd.c and are not defined elsewhere (used as literal numbers in cmd.c).
const uint8_t NSWC_CMD_RXFILTER_RESULT_NOT_CONSUMED = 0x00;
const uint8_t NSWC_CMD_RXFILTER_RESULT_CONSUMED = 0x01;

//static
uint8_t
CNetServicesImpl::NSWCoreCmdRxFilter
(Msg_Rx_Type *rx_ptr
 )
{
    uint32_t dwConsumerCount = 0UL;
    uint16_t wListenerIndex;

    if (NULL != m_pTheNetServicesImpl)
    {
        if (0 < m_pTheNetServicesImpl->m_wEventListenersEnd)
        {
            CMOSTMessage message(rx_ptr->Length);
            message.DeviceID(rx_ptr->Src_Adr);
            message.FBlockID(rx_ptr->FBlock_ID);
            message.InstanceID(rx_ptr->Inst_ID);
            message.FunctionID(rx_ptr->Func_ID);
            message.OpType(rx_ptr->Operation);
            message.SenderIsNetwork();
            message.Transport(MOSTMESSAGE_TRANSPORT_CONTROL_CHANNEL);

            switch (rx_ptr->Rcv_Type)
            {
            case MSG_RCV_TYPE_BROADCAST:
                message.AddressingType(MOSTMESSAGE_ADDRESSING_BROADCAST);
                break;
            case MSG_RCV_TYPE_GROUPCAST:
                message.AddressingType(MOSTMESSAGE_ADDRESSING_GROUPCAST);
                break;
            case MSG_RCV_TYPE_NODE_POS:
                message.AddressingType(MOSTMESSAGE_ADDRESSING_PHYSICAL);
                break;
            default:
                message.AddressingType(MOSTMESSAGE_ADDRESSING_LOGICAL);
                break;
            }

            if ((0 < message.PayloadLength()) && (NULL != rx_ptr->Data))
            {
                uint8_t *pucPayload = message.Payload();

                K2L_DEBUGCHK(NULL != pucPayload); // TODO: 0 < PayloadLength && NULL == pucPayload is an error, isn't it? The assert is commented errorneously!
                if (NULL != pucPayload)
                {
                    memcpy(pucPayload, rx_ptr->Data, rx_ptr->Length);
                }
            }

            for (wListenerIndex = 0
                    ; wListenerIndex < m_pTheNetServicesImpl->m_wEventListenersEnd
                    ; wListenerIndex++
                    )
            {
                if (!m_pTheNetServicesImpl->m_eventListeners[wListenerIndex].m_isMAC)
                {
                    if (NULL != m_pTheNetServicesImpl->m_eventListeners[wListenerIndex].m_pNetServicesEvent)
                    {
                        m_pTheNetServicesImpl
                           ->m_eventListeners[wListenerIndex].m_pNetServicesEvent->OnMessageReceived(message, /* ref */ dwConsumerCount);
                    }
                }
            }

            // if the messages was not consumed, we send the message to ACS
	    if (NULL != m_pTheNetServicesImpl->m_macEventListener)
	    {
	        if (0 == dwConsumerCount)
		{
		    m_pTheNetServicesImpl->m_macEventListener->OnMessageReceived(message, dwConsumerCount);
		}
            }
        }
    }

    if (0 == dwConsumerCount)
    {
        return NSWC_CMD_RXFILTER_RESULT_NOT_CONSUMED;
    }
    else
    {
        return NSWC_CMD_RXFILTER_RESULT_CONSUMED;
    }
}

//static
void
CNetServicesImpl::NSWCoreNetServicesAvailable
(
 )
{
    if (NULL != m_pTheNetServicesImpl)
    {
        uint16_t wListenerIndex;
        for (wListenerIndex = 0
                ; wListenerIndex < m_pTheNetServicesImpl->m_wEventListenersEnd
                ; wListenerIndex++
                )
        {
            if (NULL != m_pTheNetServicesImpl->m_eventListeners[wListenerIndex].m_pNetServicesEvent)
            {
                m_pTheNetServicesImpl->m_eventListeners[wListenerIndex].m_pNetServicesEvent->OnNetservicesAPIAvailable();
            }
        }
    }
}

//static
void
CNetServicesImpl::NSWCoreNetServicesNotAvailable
(
 )
{
    if (NULL != m_pTheNetServicesImpl)
    {
        uint16_t wListenerIndex;
        for (wListenerIndex = 0
                ; wListenerIndex < m_pTheNetServicesImpl->m_wEventListenersEnd
                ; wListenerIndex++
                )
        {
            if (NULL != m_pTheNetServicesImpl->m_eventListeners[wListenerIndex].m_pNetServicesEvent)
            {
                m_pTheNetServicesImpl->m_eventListeners[wListenerIndex].m_pNetServicesEvent->OnNetservicesAPIUnavailable();
            }
        }
    }
}

/*static*/
void
CNetServicesImpl::NSWCoreOnLongUnlock
(
 )
{
    if (NULL != m_pTheNetServicesImpl)
    {
        uint16_t wListenerIndex;
        for (wListenerIndex = 0
                ; wListenerIndex < m_pTheNetServicesImpl->m_wEventListenersEnd
                ; wListenerIndex++
                )
        {
            if (NULL != m_pTheNetServicesImpl->m_eventListeners[wListenerIndex].m_pNetServicesEvent)
            {
                m_pTheNetServicesImpl->m_eventListeners[wListenerIndex].m_pNetServicesEvent->OnLongUnlock();
            }
            
        }
    }
}

/*static*/
void
CNetServicesImpl::NSWCoreOnShortUnlock
(
 )
{
    if (NULL != m_pTheNetServicesImpl)
    {
        uint16_t wListenerIndex;
        for (wListenerIndex = 0
                ; wListenerIndex < m_pTheNetServicesImpl->m_wEventListenersEnd
                ; wListenerIndex++
                )
        {
            if (NULL != m_pTheNetServicesImpl->m_eventListeners[wListenerIndex].m_pNetServicesEvent)
            {
                m_pTheNetServicesImpl->m_eventListeners[wListenerIndex].m_pNetServicesEvent->OnShortUnlock();
            }
        }
    }
}

/*static*/
void
CNetServicesImpl::NSWCoreOnNetOn
(
 )
{
    LLDSetMostNetStatusOn();
    if (NULL != m_pTheNetServicesImpl)
    {
        uint16_t wListenerIndex;
        for (wListenerIndex = 0
                ; wListenerIndex < m_pTheNetServicesImpl->m_wEventListenersEnd
                ; wListenerIndex++
                )
        {
            if (NULL != m_pTheNetServicesImpl->m_eventListeners[wListenerIndex].m_pNetServicesEvent)
            {
                m_pTheNetServicesImpl->m_eventListeners[wListenerIndex].m_pNetServicesEvent->OnNetOn();
            }
        }
    }
}

/*static*/
void
CNetServicesImpl::NSWCoreOnNetOff
(
 )
{
    LLDSetMostNetStatusOff();
    if (NULL != m_pTheNetServicesImpl)
    {
        m_pTheNetServicesImpl->m_pendingMhpRxMessages.clear();
        m_pTheNetServicesImpl->m_pendingMhpTxMessages.clear();

        uint16_t wListenerIndex;
        for (wListenerIndex = 0
                ; wListenerIndex < m_pTheNetServicesImpl->m_wEventListenersEnd
                ; wListenerIndex++
                )
        {
            if (NULL != m_pTheNetServicesImpl->m_eventListeners[wListenerIndex].m_pNetServicesEvent)
            {
                m_pTheNetServicesImpl->m_eventListeners[wListenerIndex].m_pNetServicesEvent->OnNetOff();
            }
        }
    }
}

/*static*/
void
CNetServicesImpl::NSWCoreOnMPR
(MPR mpr
 )
{
    if (NULL != m_pTheNetServicesImpl)
    {
        uint16_t wListenerIndex;
        for (wListenerIndex = 0
                ; wListenerIndex < m_pTheNetServicesImpl->m_wEventListenersEnd
                ; wListenerIndex++
                )
        {
            if (NULL != m_pTheNetServicesImpl->m_eventListeners[wListenerIndex].m_pNetServicesEvent)
            {
                m_pTheNetServicesImpl->m_eventListeners[wListenerIndex].m_pNetServicesEvent->OnMPR(mpr);
            }
        }
    }
}

/*static*/
void
CNetServicesImpl::NSWCoreOnBandWidthChanged
(uint8_t availableQuadletsForPacketData
 )
{
}

#ifdef MNSV3
#define TOTAL_PACKET_LENGTH tx_ptr->TotalPacketLength
#else
#define TOTAL_PACKET_LENGTH 0
#endif

#ifdef MNSV3
#define HOLD_CON_OPT tx_ptr->HoldConOpt
#else
#define HOLD_CON_OPT 0
#endif

/*static*/
uint8_t
CNetServicesImpl::NSWCoreMhpTxStatus
(uint8_t status
 , pTMhpTx tx_ptr
 )
{
    K2L_RETAILMSG(ZONE_NSW, (K2LDEBUG_TEXT("++ Send MHP > 64KBytes: Status %02X Options %02X Priority %02X Addr %04X.%02X.%02X.%03X.%01X Length %08X Total Len %08X HoldConOpt %02X\r\n"), 
        status,
        tx_ptr->Options,
        tx_ptr->Priority,
        tx_ptr->Tgt_Adr,
        tx_ptr->FBlock_ID,
        tx_ptr->Inst_ID,
        tx_ptr->Func_ID,
        tx_ptr->Operation,
        tx_ptr->Length,
        TOTAL_PACKET_LENGTH,
        HOLD_CON_OPT
        ));

    if (NULL != m_pTheNetServicesImpl)
    {
        uint8_t key = m_pTheNetServicesImpl->m_pendingMhpTxMessages.capacity;

        switch (status)
        {
        case MHP_TX_SUCCESS:
            if (NULL != m_pTheNetServicesImpl->m_pMAC)
            {
                key = tx_ptr->TxHandle[0];
                K2L_DEBUGCHK(m_pTheNetServicesImpl->m_pendingMhpTxMessages.capacity != key); // MNS call us twice, here?
                if (m_pTheNetServicesImpl->m_pendingMhpTxMessages.exists(key))
                {
                    uint16_t wFunctionIDAndOpType = (tx_ptr->Func_ID << 4) | tx_ptr->Operation;
                    const uint8_t CH = EVENT_CHANNEL_MOST_HIGH;
                    const uint8_t ID = EVENT_ID_MOST_HIGH_TRANSMIT_COMPLETE;
                    uint8_t Tags[4];
                    Tags[0] = tx_ptr->FBlock_ID;
                    Tags[1] = tx_ptr->Inst_ID;
                    Tags[2] = wFunctionIDAndOpType >> 8;
                    Tags[3] = wFunctionIDAndOpType & 0x00FF;
                    m_pTheNetServicesImpl->m_pMAC->SendK2LEvent(CH, ID, 4, Tags, 0, NULL);
                }
            }
            break;

        case MHP_TX_END_READY:
            {
                // End of the MHP connection.
                // TODO: Send additional event to the application (nice to have) - Review SMSC 09.09.2010.
                uint16_t wFunctionIDAndOpType = (tx_ptr->Func_ID << 4) | tx_ptr->Operation;
                const uint8_t CH = EVENT_CHANNEL_MOST_HIGH;
                const uint8_t ID = EVENT_ID_MOST_HIGH_TX_CONNECTION_END;
                uint8_t Tags[4];
                Tags[0] = tx_ptr->FBlock_ID;
                Tags[1] = tx_ptr->Inst_ID;
                Tags[2] = wFunctionIDAndOpType >> 8;
                Tags[3] = wFunctionIDAndOpType & 0x00FF;
                m_pTheNetServicesImpl->m_pMAC->SendK2LEvent(CH, ID, 4, Tags, 0, NULL);
            }
            break;

        default:
            if (NULL != m_pTheNetServicesImpl->m_pMAC)
            {
                key = tx_ptr->TxHandle[0];

                if (m_pTheNetServicesImpl->m_pendingMhpTxMessages.exists(key))
                {
                    uint16_t wFunctionIDAndOpType = (tx_ptr->Func_ID << 4) | tx_ptr->Operation;
                    const uint8_t CH = EVENT_CHANNEL_MOST_HIGH;
                    const uint8_t ID = EVENT_ID_MOST_HIGH_TRANSMIT_FAILED;
                    uint8_t Tags[4];
                    Tags[0] = tx_ptr->FBlock_ID;
                    Tags[1] = tx_ptr->Inst_ID;
                    Tags[2] = wFunctionIDAndOpType >> 8;
                    Tags[3] = wFunctionIDAndOpType & 0x00FF;
                    m_pTheNetServicesImpl->m_pMAC->SendK2LEvent(CH, ID, 4, Tags, 0, NULL);

                    //CR14 - Trigger TxFailed event
                    m_pTheNetServicesImpl->m_pMAC->TxFailed(
                        tx_ptr->Tgt_Adr,
                        tx_ptr->FBlock_ID,
                        tx_ptr->Inst_ID
                        );

                }
            }
            break;

        }

        if (m_pTheNetServicesImpl->m_pendingMhpTxMessages.capacity != key)
        {
            // Remove the entry in any case - this message is history.
            m_pTheNetServicesImpl->m_pendingMhpTxMessages.remove(key);
            tx_ptr->TxHandle[0] = m_pTheNetServicesImpl->m_pendingMhpTxMessages.capacity;
        }
    }

    return 0;
}

uint16_t 
CNetServicesImpl::NSWCoreMhpRxGetBufferSize(uint8_t fblockID, uint8_t instanceID, uint16_t functionID, uint8_t operation)
{
    uint16_t bufferSize = m_pTheNetServicesImpl->m_mhpRxBufferConfiguration.FindBufferSize
            ( fblockID
             , instanceID
             , functionID
             , operation
             );
    return bufferSize;
}

/*static*/
uint8_t *
CNetServicesImpl::NSWCoreMhpRxBufferAllocator
(uint8_t block_type
 , TMhpRx *msg_ptr
 )
{
#if MAX_MHP_RX_HANDLE < 1
#error You have to define MAX_MHP_RX_HANDLE in adjustmh.h to at least 1 byte to meet the requirements of this implementation.
#endif
    K2L_RETAILMSG
            (ZONE_NSW, (K2LDEBUG_TEXT("NSWCoreMhpRxBufferAllocator (%02X %02X %02X %02X)")
             , msg_ptr->FBlock_ID, msg_ptr->Inst_ID, msg_ptr->Func_ID, msg_ptr->Operation));

    uint8_t *pBuffer = NULL;
    K2L_DEBUGCHK(NULL != msg_ptr);
    if ((NULL != msg_ptr) && (NULL != m_pTheNetServicesImpl))
    {
        // Following block is handling DSIHold functionality for functions between 0x400 and 0x4FF.
    	if (msg_ptr->FBlock_ID == 0x0F)
    	{
    		if ((msg_ptr->Func_ID >= 0x400) && (msg_ptr->Func_ID <= 0x4FF))
    		{
    			uint8_t index = msg_ptr->Func_ID - 0x400;
    			if (m_pTheNetServicesImpl->m_etFBlockService.HoldState(index))
    			{
                    msg_ptr->BufSize = 0;
                    msg_ptr->RxHandle[0] = m_pTheNetServicesImpl->m_pendingMhpRxMessages.capacity;
                    return NULL;
    			}
    			else
    			{
    				//Business as usual. Nothing to be done in this case.
    			}
    		}
    	}

        uint16_t bufferSize = m_pTheNetServicesImpl->m_mhpRxBufferConfiguration.FindBufferSize
                (msg_ptr->FBlock_ID
                 , msg_ptr->Inst_ID
                 , msg_ptr->Func_ID
                 , msg_ptr->Operation
                 );
        if (0 < bufferSize)
        {
            if (!m_pTheNetServicesImpl->m_pendingMhpRxMessages.full())
            {
                CMOSTMessage message(bufferSize);
                message.FBlockID(msg_ptr->FBlock_ID);
                message.InstanceID(msg_ptr->Inst_ID);
                message.FunctionID(msg_ptr->Func_ID);
                message.OpType(msg_ptr->Operation);

                pBuffer = message.Payload();
                if (NULL != pBuffer) // Only fails in Out of memory situations.
                {
                    //TODO: See comment inside NetServicesCallbacks.cpp -> MhpGetBufIntf()
                    msg_ptr->BufSize = message.PayloadLength();
                    //Leave following line after cleanup. This is public field. This is OK.
                    msg_ptr->RxHandle[0] = m_pTheNetServicesImpl->m_pendingMhpRxMessages.add(message);
                    K2L_DEBUGCHK(msg_ptr->RxHandle[0] < m_pTheNetServicesImpl->m_pendingMhpRxMessages.capacity);
                }
                else
                {
                    m_pTheNetServicesImpl->MhpRxFailed
                            (msg_ptr->FBlock_ID
                             , msg_ptr->Inst_ID
                             , msg_ptr->Func_ID
                             , msg_ptr->Operation
                             );
                    msg_ptr->BufSize = 0;
                    msg_ptr->RxHandle[0] = m_pTheNetServicesImpl->m_pendingMhpRxMessages.capacity;
                }
            }
            else
            {
                m_pTheNetServicesImpl->MhpRxFailed
                        (msg_ptr->FBlock_ID
                         , msg_ptr->Inst_ID
                         , msg_ptr->Func_ID
                         , msg_ptr->Operation
                         );
                msg_ptr->BufSize = 0;
                msg_ptr->RxHandle[0] = m_pTheNetServicesImpl->m_pendingMhpRxMessages.capacity;
            }
        }
        else
        {
            m_pTheNetServicesImpl->MhpRxRefused
                    (msg_ptr->FBlock_ID
                     , msg_ptr->Inst_ID
                     , msg_ptr->Func_ID
                     , msg_ptr->Operation
                     );
            msg_ptr->BufSize = 0;
            msg_ptr->RxHandle[0] = m_pTheNetServicesImpl->m_pendingMhpRxMessages.capacity;
        }
    }

    if (pBuffer == NULL)
    {
        K2L_RETAILMSG(K2L_ZONE_ERROR, (K2LDEBUG_TEXT("CNetServicesImpl::NSWCoreMhpRxBufferAllocator(FAILED)\r\n")));
    }
    return pBuffer;
}

/*static*/
void
CNetServicesImpl::NSWCoreMhpRxComplete
(uint8_t status
 , pTMhpRx rx_ptr
 )
{
    uint8_t key = m_pTheNetServicesImpl->m_pendingMhpRxMessages.capacity;
    
    if (NULL != m_pTheNetServicesImpl && NULL != rx_ptr)
    {
        key = rx_ptr->RxHandle[0];
    }

    K2L_RETAILMSG(ZONE_NSW, (K2LDEBUG_TEXT("CNetServicesImpl::NSWCoreMhpRxComplete(status %02X SegIDRx %02X BlockCntRel %04X PacketCnt %04X Addr %03X.%02X.%02X.%03X.%01X Length %08X)\r\n"), 
        status, 
        rx_ptr->SegIDRx, 
        rx_ptr->BlockCntRel,
        rx_ptr->PacketCnt,
        rx_ptr->Src_Adr,
        rx_ptr->FBlock_ID,
        rx_ptr->Inst_ID,
        rx_ptr->Func_ID,
        rx_ptr->Operation,
        rx_ptr->Length
        ));

    switch (status)
    {
    case MHP_RX_CLOSE:
        break;

    case MHP_RX_SUCCESS:
        if (NULL != m_pTheNetServicesImpl && NULL != rx_ptr)
        {
            if (m_pTheNetServicesImpl->m_pendingMhpRxMessages.exists(key))
            {
                CMOSTMessage & rMessage = m_pTheNetServicesImpl->m_pendingMhpRxMessages.get(key);
                K2L_DEBUGCHK(rMessage.FBlockID() == rx_ptr->FBlock_ID);
                K2L_DEBUGCHK(rMessage.InstanceID() == rx_ptr->Inst_ID);
                K2L_DEBUGCHK(rMessage.FunctionID() == rx_ptr->Func_ID);
                K2L_DEBUGCHK(rMessage.OpType() == rx_ptr->Operation);

                rMessage.DeviceID(rx_ptr->Src_Adr);
                rMessage.Transport(MOSTMESSAGE_TRANSPORT_MOST_HIGH);

                //TODO: Where do we get receive type from for MOST high
                rMessage.AddressingType(MOSTMESSAGE_ADDRESSING_LOGICAL);

                switch(rx_ptr->SegIDRx)
                {
                case 0x01:
                    rMessage.MHPType(MOSTMESSAGE_MHPTYPE_FIRSTSEGMENT);
                    break;
                case 0x02:
                    rMessage.MHPType(MOSTMESSAGE_MHPTYPE_NEXTSEGMENT);
                    break;
                case 0x03:
                    rMessage.MHPType(MOSTMESSAGE_MHPTYPE_LASTSEGMENT);
                    break;
                default:
                    rMessage.MHPType(MOSTMESSAGE_MHPTYPE_UNSEGMENTED);
                    break;
                }

                rMessage.SenderIsNetwork();

                K2L_DEBUGCHK(rx_ptr->Length <= rMessage.PayloadLength());
                rMessage.TrimPayloadLength(rx_ptr->Length);

                uint32_t consumerCount = 0; // Not used in case of MOST High.
                uint16_t wListenerIndex;
                for (wListenerIndex = 0
                        ; wListenerIndex < m_pTheNetServicesImpl->m_wEventListenersEnd
                        ; wListenerIndex++
                        )
                {
                    if (!m_pTheNetServicesImpl->m_eventListeners[wListenerIndex].m_isMAC)
                    {
                        if (NULL != m_pTheNetServicesImpl->m_eventListeners[wListenerIndex].m_pNetServicesEvent)
                        {
                            m_pTheNetServicesImpl
                               ->m_eventListeners[wListenerIndex].m_pNetServicesEvent->OnMessageReceived(rMessage, /* ref */ consumerCount);
                        }
                    }
                }

		// if the messages was not consumed, we send the message to ACS
		if (NULL != m_pTheNetServicesImpl->m_macEventListener)
		{
		    if (0 == consumerCount)
		    {
		        m_pTheNetServicesImpl->m_macEventListener->OnMessageReceived(rMessage, consumerCount);
		    }
                }
            }
            else
            {
                K2L_DEBUGCHK(false);
            }
        }
        break;

    default: // error case
        if (NULL != m_pTheNetServicesImpl && NULL != rx_ptr)
        {
            m_pTheNetServicesImpl->MhpRxFailed
                    (rx_ptr->FBlock_ID
                     , rx_ptr->Inst_ID
                     , rx_ptr->Func_ID
                     , rx_ptr->Operation
                     );
        }
        break;
    }

    if (m_pTheNetServicesImpl->m_pendingMhpRxMessages.capacity != key)
    {
        // Cleanup in all cases. This message is history.
        m_pTheNetServicesImpl->m_pendingMhpRxMessages.remove(key);
        rx_ptr->BufSize = 0;
        rx_ptr->RxHandle[0] = m_pTheNetServicesImpl->m_pendingMhpRxMessages.capacity;
    }
}

/*static*/
uint8_t
CNetServicesImpl::NSWCoreAMSTxStatus
(byte status
 , TMsgTx *msg_ptr
 )
{
    K2L_DEBUGCHK(NULL != msg_ptr);
    if (NULL != msg_ptr)
    {
        switch (status)
        {
        case XMIT_WRONGTARGET: // Transmission failed. No response under choosen target address.
        {
            K2L_DEBUGCHK(NULL != m_pTheNetServicesImpl->m_pMAC);
            if (NULL != m_pTheNetServicesImpl->m_pMAC)
            {
                //CR14 - Trigger TxFailed event
                m_pTheNetServicesImpl->m_pMAC->TxFailed(
                                                        msg_ptr->Tgt_Adr,
                                                        msg_ptr->FBlock_ID,
                                                        msg_ptr->Inst_ID
                                                        );
            }
        }
            break;

        case XMIT_SUCCESS: // Transmission of all telegrams OK
        case XMIT_CRC: // Incorrect CRC.
        case XMIT_BUF: // Receive buffer of a target device full.
        case XMIT_TIMEOUT: // Timeout error. No response from lower layer.
        case XMIT_FAILED: // Transmission failed
        case XMIT_NET_OFF: // NET OFF
        case XMIT_FRMT: // Protocol error in the port message.
        case XMIT_SYNC: // Sync. error between INIC and PMS. The PMS has been resynchronized.
        case XMIT_INTERNAL: // Internal error.
        case XMIT_NO_ICM: // You tried to send an ICM message which is reserved for the service of MOST NetServices only.
        case XMIT_PROTECTED_MODE: // Protected mode

        default:
            break;
        }
    }

    return MSG_TX_FREE;
}
